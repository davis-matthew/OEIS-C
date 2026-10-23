/*
 * A126350  Triangle read by rows: matrix product of the binomial coefficients
 *          with the Stirling numbers of the second kind.
 * https://oeis.org/A126350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ∑_{k=i}^{j} binomial(k-1, i-1) · S2(j, k) , where j is
 *            the smallest integer with j·(j+1)/2 ≥ n, i = j – (n – (j−1)·j/2)
 *            + 1 (rows are counted from 1)
 * Range:     u8 n <= 18, u16 n <= 41, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                 /* malloc, free */
#include "../utils/binomial.h"      /* seq_t binomial(idx_t a, idx_t b) */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1 */

    /* ---------- find column j ---------- */
    idx_t j = 1;
    while (j * (j + 1) / 2 < n) ++j;           /* j ≤ ~√(2n) for n ≤ 10000 */

    idx_t offset = (j - 1) * j / 2;            /* entries before column j   */
    idx_t r = n - offset;                     /* rank within column        */
    idx_t i = j - r + 1;                      /* row index (1‑based)       */

    /* ---------- compute Stirling numbers S2(j, k) 1 ≤ k ≤ j ----------
       Using two arrays of size j+1 to avoid in‑place update bugs.   */
    seq_t *prev = (seq_t *)malloc((j + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((j + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        free(prev); free(curr);
        return 0;
    }

    for (idx_t k = 0; k <= j; ++k) prev[k] = 0;
    prev[0] = 1;                              /* S2(0,0) = 1 */

    for (idx_t row = 1; row <= j; ++row) {
        curr[0] = 0;
        for (idx_t k = 1; k <= row; ++k) {
            curr[k] = (k * prev[k]) + prev[k - 1];
        }
        /* zero out the rest (not strictly needed) */
        for (idx_t k = row + 1; k <= j; ++k) curr[k] = 0;
        /* swap prev and curr for next iteration */
        seq_t *tmp = prev; prev = curr; curr = tmp;
    }

    /* ---------- compute the weighted sum ----------
       Now prev[k] == S2(j, k).  */
    seq_t sum = 0;
    for (idx_t k = i; k <= j; ++k) {
        seq_t bin = binomial(k - 1, i - 1);   /* binomial(k-1, i-1) */
        sum = sum + (bin * prev[k]);          /* accumulate */
    }

    free(prev);
    free(curr);
    return sum;
}
