/*
 * A058807  a(n) = Product_{k=1..n} s(n,k), where s(n,k) is unsigned Stirling
 *          number of the first kind. (s(n,k) = number of permutations of n
 *          elements which contain exactly k cycles.)
 * https://oeis.org/A058807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=1..n} s(n,k) where s(n,k) is the unsigned
 *            Stirling number of the first kind
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Undefined for n=0, but return 0 */

    /* Dynamic arrays to hold one row of Stirling numbers */
    size_t size = (size_t)n + 1;
    seq_t *prev = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(size, sizeof(seq_t));
    if (!prev || !curr) return 0; /* Allocation failure */

    prev[0] = 1; /* s(0,0) = 1, other entries are 0 by calloc */

    seq_t result = 0;
    for (size_t i = 1; i <= n; ++i) {
        curr[0] = 0;
        /* Compute s(i,k) for k=1..i */
        for (size_t k = 1; k <= i; ++k) {
            seq_t term1 = prev[k - 1];
            seq_t term2 = 0;
            if (k <= i - 1) {
                /* Multiply by (i-1); cast to seq_t to avoid overflow of idx_t */
                term2 = (seq_t)(i - 1) * prev[k];
            }
            curr[k] = term1 + term2;
        }

        /* Multiply all s(i,k) to form a(i) */
        seq_t prod = 1;
        for (size_t k = 1; k <= i; ++k) {
            prod *= curr[k];
        }
        if (i == n) result = prod;

        /* Prepare for next iteration: swap prev and curr */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    free(prev);
    free(curr);
    return result;
}
