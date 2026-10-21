/*
 * A056118  a(n) = (11*n+5)*(n+4)*(n+3)*(n+2)*(n+1)/120.
 * https://oeis.org/A056118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n+5)*(n+1)*(n+2)*(n+3)*(n+4)/120
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 134, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* Compute binomial (n+4 choose 4) via multiplicative formula */
    seq_t binom = 1;
    for (idx_t i = 1; i <= 4; ++i) {
        binom = (seq_t)(((__int128)binom * (n + i)) / i);
    }
    /* Compute the sequence value */
    seq_t val = (seq_t)(((__int128)binom * (11 * n + 5)) / 5);
    return val;
}
