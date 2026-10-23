/*
 * A047596  Numbers that are congruent to {2, 3, 4, 5} mod 8.
 * https://oeis.org/A047596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + (2 + (n-1)%4) (offset 1)
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* the sequence is defined for n >= 1; for n < 1 we return 0 */
    if (n < 1) return (seq_t)0;

    /* compute quotient and remainder of (n-1) divided by 4 */
    idx_t q = (n - 1) / 4;   /* floor((n-1)/4) */
    int r = (int)((n - 1) % 4); /* 0 <= r <= 3 */

    /* result is 8*q + (2 + r) */
    seq_t result = (seq_t)8 * (seq_t)q + (seq_t)(2 + r);
    return result;
}
