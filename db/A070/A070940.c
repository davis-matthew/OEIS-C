/*
 * A070940  Number of digits that must be counted from left to right to reach
 *          the last 1 in the binary representation of n.
 * https://oeis.org/A070940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, tabf
 * Formula:   a(n) = (floor(log2(n)) + 1) - count_trailing_zeros(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes A070940: number of digits that must be counted from left to right
 * to reach the last (least significant) 1 in the binary representation of n.
 * Assumes n >= 1. */
seq_t a(idx_t n)
{
    /* Count bits */
    unsigned long long v = (unsigned long long)n;
    unsigned bits = 64 - __builtin_clzll(v);         /* floor(log2(v)) + 1 */

    /* Count trailing zeros */
    unsigned tz = 0;
    unsigned long long tmp = v;
    while ((tmp & 1ULL) == 0) {
        tz++;
        tmp >>= 1;
    }

    return (seq_t)(bits - tz);
}
