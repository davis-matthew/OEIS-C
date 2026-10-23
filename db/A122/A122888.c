/*
 * A122888  Triangle, read by rows, where row n lists the coefficients of x^k,
 *          k=1..2^n, in the n-th iteration of (x + x^2) for n>=0.
 * https://oeis.org/A122888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) is the coefficient of x^k in the n‑th iteration of (x +
 *            x^2), i.e. F_{n+1}(x) = F_n(x+x^2) with F_0(x)=x. The sequence
 *            a(n) is obtained by reading the triangle row‑by‑row
 * Range:     u8 n <= 21, u16 n <= 42, u32 n <= 82, u64 n <= 164,
 *            u128 n <= 328, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- required utilities ---------- */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

/* ---------- internal cache ---------- */
static seq_t **cache = NULL;
static size_t cache_len = 0;   /* number of rows stored (max index+1) */

/* ----- helper: compute floor(log2(n)) for n>=1 ----- */
static idx_t floor_log2(idx_t n)
{
    idx_t r = 0;
    while (n >>= 1) r++;
    return r;
}

/* ----- build all rows up to 'row' inclusively ----- */
static void ensure_row(idx_t row)
{
    if (cache_len > row) return;          /* already available */

    /* Grow the pointer array if required. */
    if (cache_len == 0) {
        cache = (seq_t **)malloc(sizeof(seq_t *) * (row + 1));
    } else if (cache_len <= row) {
        cache = (seq_t **)realloc(cache, sizeof(seq_t *) * (row + 1));
    }

    /* Build each missing row from the previous one. */
    for (idx_t r = cache_len; r <= row; ++r) {
        size_t prev_len = (r == 0) ? 0 : ((size_t)1ULL << (r - 1));
        size_t new_len  = ((size_t)1ULL << r);

        seq_t *new_row = (seq_t *)calloc(new_len, sizeof(seq_t));
        if (!new_row) exit(1);   /* allocation failure */

        if (r == 0) {            /* F_0(x) = x  → coefficient of x^1 is 1 */
            new_row[0] = 1;
        } else {
            seq_t *prev = cache[r - 1];
            for (idx_t i = 0; i < prev_len; ++i) {
                idx_t old_exp   = i + 1;          /* exponent of current term */
                seq_t coeff     = prev[i];        /* coefficient of x^{old_exp} */

                /* Expand (x + x^2)^{old_exp} */
                for (idx_t j = 0; j <= old_exp; ++j) {
                    seq_t binom = binomial(old_exp, j);   /* C(old_exp, j) */
                    seq_t term  = coeff * binom;          /* coefficient contribution */
                    size_t pos  = (size_t)(old_exp + j - 1);/* index for x^{old_exp+j} */
                    new_row[pos] += term;
                }
            }
        }

        cache[r] = new_row;   /* store the newly computed row */
    }

    cache_len = row + 1;
}

/* ---------- sequence entry function ---------- */
seq_t a(idx_t n)
{
    /* Find the row that contains the n‑th entry (triangular indexing). */
    idx_t row = floor_log2(n + 1);            /* because 2^row-1 <= n < 2^{row+1}-1 */
    ensure_row(row);

    /* Position of n inside its row (zero‑based). */
    idx_t start = ((idx_t)1ULL << row) - 1;
    idx_t k     = n - start;                  /* 0‑based index within row */

    return cache[row][k];
}
