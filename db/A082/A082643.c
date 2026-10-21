/*
 * A082643  a(n) = ceiling(n*(n+1/2)/2).
 * https://oeis.org/A082643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 - floor(n/4) (OEIS A082643)
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Convert index to sequence type early */
    seq_t nn = (seq_t)n;
    /* Triangular number n*(n+1)/2 */
    seq_t tri = nn * (nn + 1) / 2;
    /* Floor division n/4 (using idx_t division, then cast) */
    seq_t quarter = (seq_t)(n / 4);
    return tri - quarter;
}
