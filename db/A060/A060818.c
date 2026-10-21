/*
 * A060818  a(n) = 2^(n - HammingWeight(n)) = 2^(n - BitCount(n)) = 2^(n -
 *          A000120(n)).
 * https://oeis.org/A060818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 2^(n - popcount(n)) = 2^(⌊n/2⌋+⌊n/4⌋+⌊n/8⌋+…)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the exponent of 2: sum of floor(n/2^k) */
    idx_t m = n;
    uint64_t exp = 0;
    while (m > 0) {
        m >>= 1;          /* m = floor(m/2) */
        exp += m;         /* add floor(n/2^k) to exp */
    }

    /* Compute 2^exp in a generic seq_t type */
    seq_t result = 1;
    for (uint64_t i = 0; i < exp; ++i) {
        result *= 2;
    }
    return result;
}
