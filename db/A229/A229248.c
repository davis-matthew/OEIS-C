/*
 * A229248  Number of set partitions of {1,...,n} with largest set of size 6.
 * https://oeis.org/A229248
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of set partitions of {1,…,n} with largest block
 *            size 6. Computation: a(n) = S(n,≤6) – S(n,≤5), where S(m,≤k)
 *            satisfies S(0,≤k)=1, and for m≥1, S(m,≤k)=∑_{i=1}^{min(k,m)}
 *            binom(m‑1,i‑1)·S(m‑i,≤k)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 16, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a5 = NULL;   /* S(·,≤5) */
static seq_t *a6 = NULL;   /* S(·,≤6) */
static idx_t maxN = 0;     /* largest n for which arrays are filled */

/* Ensure the DP arrays are large enough to store values up to n. */
static void ensure_capacity(idx_t n)
{
    if (n <= maxN)
        return;

    size_t new_size = (size_t)n + 1;
    seq_t *new_a5 = (seq_t *)realloc(a5, new_size * sizeof(seq_t));
    seq_t *new_a6 = (seq_t *)realloc(a6, new_size * sizeof(seq_t));
    if (!new_a5 || !new_a6) abort();  /* failure, terminate */

    a5 = new_a5;
    a6 = new_a6;

    /* initialize new entries */
    for (idx_t i = maxN + 1; i <= n; ++i) {
        a5[i] = 0;
        a6[i] = 0;
    }

    if (maxN == 0) {
        /* base case for unrestricted partitions */
        a5[0] = 1;
        a6[0] = 1;
    }

    /* compute DP rows from the old maximum up to n */
    for (idx_t m = maxN + 1; m <= n; ++m) {
        seq_t sum6 = 0, sum5 = 0;
        idx_t limit6 = m < 6 ? m : 6;
        idx_t limit5 = m < 5 ? m : 5;

        for (idx_t i = 1; i <= limit6; ++i) {
            /* binom(m-1, i-1) */
            seq_t binom = 1;
            for (idx_t d = 1; d <= i - 1; ++d) {
                binom *= (seq_t)(m - d);   /* multiply by (m - d) */
                binom /= (seq_t)d;         /* divide by d (exact division) */
            }
            sum6 += a6[m - i] * binom;
        }
        for (idx_t i = 1; i <= limit5; ++i) {
            /* binom(m-1, i-1) */
            seq_t binom = 1;
            for (idx_t d = 1; d <= i - 1; ++d) {
                binom *= (seq_t)(m - d);
                binom /= (seq_t)d;
            }
            sum5 += a5[m - i] * binom;
        }
        a6[m] = sum6;
        a5[m] = sum5;
    }

    maxN = n;
}

seq_t a(idx_t n)
{
    if (n < 6)
        return 0;

    ensure_capacity(n);
    return a6[n] - a5[n];
}
