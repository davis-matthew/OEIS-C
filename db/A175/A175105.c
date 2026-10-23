/*
 * A175105  Triangle T(n,k) read by rows. T(n,1)=1; T(n,k) = Sum_{i=1..k-1} (
 *          T(n-i,k-1) + T(n-i,k) ), k>1.
 * https://oeis.org/A175105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle T(n,k) T(n,1) = 1; T(n,k) = Σ_{i=1}^{k-1} ( T(n-i,k-1)
 *            + T(n-i,k) ), k > 1. a(n) is the n‑th element when the triangle
 *            is read row‑by‑row. Example: a(1)=1, a(2)=1, a(3)=1, a(4)=1,
 *            a(5)=2, …
 * Range:     u8 n <= 39, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* External declaration of the sequence function */
seq_t a(idx_t n);

/*--------------------- internal state -----------------------------------*/

static seq_t *sequence = NULL;   /* flattened triangle (row‑by‑row) */
static size_t seq_len   = 0;     /* number of terms already stored */
static size_t seq_cap   = 0;     /* capacity of 'sequence'            */

static seq_t **rows = NULL;      /* array of pointers to each row */
static size_t rows_cnt = 0;      /* number of rows stored            */
static size_t rows_cap = 0;      /* capacity of 'rows'               */

/* Ensure 'sequence' has room for 'need' additional items */
static void ensure_seq_capacity(size_t need)
{
    size_t required = seq_len + need;
    if (required > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < required)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(sequence, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        sequence = tmp;
        seq_cap   = new_cap;
    }
}

/* Allocate a new row of length 'len' (indices 1..len) */
static seq_t *new_row(idx_t len)
{
    seq_t *row = (seq_t *)malloc((len + 1) * sizeof(seq_t));
    if (!row) abort();
    return row;
}

/* Compute rows until we have at least 'target' terms in the flattened
   sequence.  The function is thread‑unsafe, but the problem statement
   does not require thread safety. */
static void ensure_terms(idx_t target)
{
    while ((idx_t)seq_len < target) {
        idx_t r = rows_cnt + 1;           /* next row number           */
        seq_t *row = new_row(r);

        /* T(r,1) = 1 */
        row[1] = (seq_t)1;

        /* Compute columns k = 2 .. r */
        for (idx_t k = 2; k <= r; ++k) {
            seq_t sum = 0;
            for (idx_t i = 1; i < k; ++i) {
                idx_t m = r - i;                 /* row index n-i       */
                if (k - 1 <= m)                  /* there is T(m,k-1)  */
                    sum += rows[m-1][k-1];
                if (k <= m)                      /* there is T(m,k)    */
                    sum += rows[m-1][k];
            }
            row[k] = sum;
        }

        /* Resize the rows array if necessary */
        if (rows_cnt + 1 > rows_cap) {
            size_t new_rows_cap = rows_cap ? rows_cap * 2 : 8;
            seq_t **tmp = (seq_t **)realloc(rows,
                                            (new_rows_cap + 1) * sizeof(seq_t*));
            if (!tmp) abort();
            rows     = tmp;
            rows_cap = new_rows_cap;
        }

        /* Store the new row */
        rows[rows_cnt] = row;
        rows_cnt++;

        /* Append row values to the flattened sequence */
        ensure_seq_capacity(r);
        for (idx_t k = 1; k <= r; ++k)
            sequence[seq_len++] = row[k];
    }
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined in OEIS – return 0 to avoid UB */
        return (seq_t)0;
    ensure_terms(n);
    return sequence[n - 1];
}
