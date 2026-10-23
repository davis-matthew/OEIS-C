/*
 * A123296  Number of permutations of n distinct letters (ABCD...) each of
 *          which appears 5 times and having n-2 fixed points.
 * https://oeis.org/A123296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25 * n * (n + 1) / 2
 * Range:     u8 n <= 4, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* convert index to seq_t before any arithmetic */
    seq_t nn = (seq_t)n;
    /* compute the triangular number n*(n+1)/2 */
    seq_t tri = nn * (nn + 1) / 2;
    /* multiply by 25 and return */
    return tri * 25;
}
