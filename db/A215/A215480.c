/*
 * A215480  Characteristic function of numbers n with exactly two distinct
 *          prime factors.
 * https://oeis.org/A215480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Characteristic function of numbers with exactly two distinct prime factors */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    int distinct = 0;

    /* Factor 2 */
    if ((m & 1u) == 0) {
        distinct++;
        while ((m & 1u) == 0) m >>= 1;
    }

    /* Factor odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            distinct++;
            while (m % p == 0) m /= p;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1) distinct++;

    return (seq_t)(distinct == 2);
}
