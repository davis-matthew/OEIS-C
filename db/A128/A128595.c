/*
 * A128595  Row sums of triangle A128592.
 * https://oeis.org/A128595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} [q^(nk + k)] Product_{j=1..n+1}
 *            (1-q^(2j-1))/(1-q)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the product modulo x^(n+1)-1 and
   extracts the coefficient of x^0, which equals the required sum. */
#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t M = n + 1;                /* modulus */
    size_t m = (size_t)M;

    /* Allocate working arrays. */
    seq_t *cur = (seq_t*)malloc(m * sizeof(seq_t));
    seq_t *next = (seq_t*)malloc(m * sizeof(seq_t));
    seq_t *cum = (seq_t*)malloc((2*m + 1) * sizeof(seq_t));

    /* Initialize with the empty product. */
    cur[0] = 1;
    for (idx_t i = 1; i < M; ++i) cur[i] = 0;
    seq_t total = 1;                /* sum of all coefficients in cur */

    for (idx_t j = 0; j <= n; ++j) {
        idx_t L = 2*j + 1;          /* length of the factor (1 + x + ... + x^(2j)) */
        int  full = (L >= M);       /* does the window wrap at least once?   */
        idx_t rem  = L % M;         /* remaining part of the window after full cycles */

        /* Build cumulative sums of cur duplicated twice. */
        cum[0] = 0;
        for (idx_t i = 0; i < 2*m; ++i) {
            cum[i+1] = cum[i] + cur[i % m];
        }

        /* Compute new coefficients. */
        seq_t total_next = 0;
        for (idx_t t = 0; t < m; ++t) {
            seq_t win = 0;
            if (rem) {
                idx_t start = t + m - rem + 1;   /* inclusive start index in doubled array */
                idx_t end   = t + m;             /* inclusive end index   */
                win = cum[end+1] - cum[start];
            }
            seq_t val = full ? (total + win) : win;
            next[t] = val;
            total_next += val;
        }

        /* Swap arrays for next iteration. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
        total = total_next;
    }

    seq_t result = cur[0];

    /* Free working memory. */
    free(cur);
    free(next);
    free(cum);

    return result;
}
