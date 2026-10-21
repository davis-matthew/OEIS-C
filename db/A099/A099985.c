/*
 * A099985  a(n) = rad(2n), where rad = A007947.
 * https://oeis.org/A099985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = rad(2n) (product of the distinct primes dividing 2n)
 * Range:     u8 n <= 128, u16 n <= 9355, u32 n <= 9355, u64 n <= 9355,
 *            u128 n <= 9355, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the radical of 2*n: product of distinct prime factors of 2*n. */
seq_t a(idx_t n)
{
    /* 2*n fits in uint64_t for the intended range of n. */
    uint64_t m = (uint64_t)n << 1;  /* m = 2*n */
    seq_t rad = 1;

    if (m == 0)           /* should never happen (offset 1), but guard anyway */
        return 0;

    /* factor out 2 */
    if ((m & 1ULL) == 0) {
        rad *= (seq_t)2;
        while ((m & 1ULL) == 0)
            m >>= 1;
    }

    /* trial division for odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            rad *= (seq_t)p;
            while (m % p == 0)
                m /= p;
        }
    }

    /* if a prime > sqrt(m) remains */
    if (m > 1)
        rad *= (seq_t)m;

    return rad;
}
