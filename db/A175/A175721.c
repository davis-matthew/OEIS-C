/*
 * A175721  Array T(n,m) read by antidiagonals: the coefficient of [x^m] of
 *          1/(-x^n + 1 - x^(1+n) + x + 3*x^(2+n) - 2*x^2 - x^(3+n)) in row n,
 *          column 1 <= m.
 * https://oeis.org/A175721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is read by antidiagonals from the array T(n,m), ** where
 *            T(n,m) is the coefficient of x^m in the power series ** 1 / (1 +
 *            x - 2x^2 - x^n - x^(n+1) + 3x^(n+2) - x^(n+3)), ** defined for n
 *            ≥ 1 and m ≥ 1. The recurrence for each row n is ** T(n,m) = –
 *            Σ_{i≥1} a_i,T(n,m-i), ** where a_i are the...
 * Range:     i8 n <= 28, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   grow_seq, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/grow_seq.h"

/* --------------------------------------------------------------------- */
struct Row {
    seq_t *buf;          /* circular buffer of computed values            */
    size_t size;         /* buffer capacity = n + 4 (or more)             */
    size_t mcount;       /* number of values already stored (t1, t2,…)    */
    size_t powers[6];    /* shift amounts for non‑zero coefficients      */
    int    coeffs[6];    /* corresponding coefficients                   */
    size_t npairs;       /* number of non‑zero terms (3,5,or 6)          */
};
/* --------------------------------------------------------------------- */

/* Global state */
static struct Row *rows = NULL;
static size_t rows_cap = 0;

/* Sequence cache */
static seq_t *seq_cache = NULL;
static size_t seq_cap  = 0;
static size_t seq_len  = 0;          /* number of terms generated so far */

/* Antidiagonal traversal state */
static size_t next_sum = 2;     /* current diagonal sum n + m          */
static size_t next_n   = 1;     /* current n on that diagonal         */

/* --------------------------------------------------------------------- */
static void ensure_rows_cap(size_t n)
{
    if (n + 1 <= rows_cap) return;
    size_t new_cap = rows_cap ? rows_cap << 1 : 4;
    while (new_cap <= n) new_cap <<= 1;
    struct Row *new_rows = (struct Row *)realloc(rows, new_cap * sizeof(struct Row));
    if (!new_rows) abort();
    for (size_t i = rows_cap; i < new_cap; ++i) {
        new_rows[i].buf     = NULL;
        new_rows[i].size    = 0;
        new_rows[i].mcount  = 0;
        new_rows[i].npairs  = 0;
    }
    rows     = new_rows;
    rows_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Retrieve T(n,k).  k==0 returns the constant term 1.
   If k > mcount, the value has not been computed yet → 0.          */
static inline seq_t get_val(const struct Row *row, size_t k)
{
    if (k == 0) return 1;
    if (k > row->mcount) return 0;
    return row->buf[(k - 1) % row->size];
}

/* --------------------------------------------------------------------- */
static void generate_up_to(idx_t limit)
{
    size_t target = (size_t)limit;
    while (seq_len < target) {
        size_t n = next_n;
        size_t m = next_sum - n;

        /* Prepare row for n */
        ensure_rows_cap(n);
        struct Row *row = &rows[n];
        if (!row->buf) {
            /* Allocate a buffer large enough to keep all needed past values */
            row->size = n + 4;                     /* safe upper bound       */
            row->buf  = seq_alloc_array(row->size);
            if (!row->buf) abort();
            row->mcount = 0;

            /* Fill coefficients depending on n */
            if (n == 1) {                          /* special case n=1         */
                row->npairs = 3;
                row->powers[0] = 2;  row->coeffs[0] = -3;
                row->powers[1] = 3;  row->coeffs[1] = 3;
                row->powers[2] = 4;  row->coeffs[2] = -1;
            } else if (n == 2) {                   /* special case n=2         */
                row->npairs = 5;
                row->powers[0] = 1;  row->coeffs[0] = 1;
                row->powers[1] = 2;  row->coeffs[1] = -3;
                row->powers[2] = 3;  row->coeffs[2] = -1;
                row->powers[3] = 4;  row->coeffs[3] = 3;
                row->powers[4] = 5;  row->coeffs[4] = -1;
            } else {                               /* general case n≥3          */
                row->npairs = 6;
                row->powers[0] = 1;  row->coeffs[0] = 1;
                row->powers[1] = 2;  row->coeffs[1] = -2;
                row->powers[2] = n;  row->coeffs[2] = -1;
                row->powers[3] = n + 1;  row->coeffs[3] = -1;
                row->powers[4] = n + 2;  row->coeffs[4] = 3;
                row->powers[5] = n + 3;  row->coeffs[5] = -1;
            }
        }

        /* Compute T(n,m) using the recurrence */
        seq_t sum = 0;
        for (size_t i = 0; i < row->npairs; ++i) {
            size_t shift = row->powers[i];
            if (m >= shift) {
                sum += (seq_t)row->coeffs[i] * get_val(row, m - shift);
            }
        }
        seq_t val = -sum;   /* because the leading coefficient of D_n is 1 */

        /* Store in global cache */
        grow_seq(&seq_cache, &seq_cap, seq_len + 1);
        seq_cache[seq_len++] = val;

        /* Store in row buffer for future use */
        row->buf[row->mcount % row->size] = val;
        row->mcount++;

        /* Advance to the next position along antidiagonals */
        ++next_n;
        if (next_n == next_sum) {
            next_n  = 1;
            ++next_sum;
        }
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, guard against 0 */
    generate_up_to(n);
    return seq_cache[(size_t)n - 1];
}
