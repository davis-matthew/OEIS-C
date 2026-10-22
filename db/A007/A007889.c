/*
 * A007889  Number of intransitive (or alternating, or Stanley) trees:
 *          vertices are [0,n] and for no i<j<k are both (i,j) and (j,k)
 *          edges.
 * https://oeis.org/A007889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=1..floor((n+1)/2)} binomial(n, 2k-1)*k^(n-1) for n
 *            >= 1, a(0) = 1 (Shevelev's formula shifted by one index; all
 *            terms are nonnegative, so no intermediate exceeds a(n))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t sum = 0;
    seq_t c = (seq_t)n;            /* binomial(n, j), starting at j = 1 */
    idx_t j = 1;
    for (idx_t k = 1; 2 * k - 1 <= n; k++) {
        /* here j == 2k-1 and c == binomial(n, j) */
        sum += c * a000248_pow((seq_t)k, n - 1);
        /* advance c to binomial(n, j+2) */
        if (j + 2 <= n) {
            c = c * (seq_t)(n - j) / (seq_t)(j + 1);
            c = c * (seq_t)(n - j - 1) / (seq_t)(j + 2);
        }
        j += 2;
    }
    return sum;
}
