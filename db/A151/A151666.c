/*
 * A151666  Number of partitions of n into distinct powers of 4.
 * https://oeis.org/A151666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if all base 4 digits of n are 0 or 1, otherwise a(n) =
 *            0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to checking that n modulo 4 is 0 or 1, and recursing on n/4. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* For a given n, iteratively examine digits in base 4. */
    idx_t m = n;
    while (m > 0) {
        if ((m % 4) == 2 || (m % 4) == 3) {
            return (seq_t)0;
        }
        m /= 4;
    }
    return (seq_t)1;
}
