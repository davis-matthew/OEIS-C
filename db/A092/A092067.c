/*
 * A092067  a(n) is the smallest number m such that m > 1 and m divides n^m +
 *          1.
 * https://oeis.org/A092067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n is odd, otherwise the smallest prime divisor of
 *            (n+1)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* find the smallest prime factor of a positive integer m (m >= 2) */
static seq_t smallest_prime_factor(uint64_t m) {
    uint64_t p;
    for (p = 2; p * p <= m; ++p) {
        if (m % p == 0) return (seq_t)p;
    }
    return (seq_t)m;  /* m is prime */
}

seq_t a(idx_t n) {
    /* n >= 1 by definition of the sequence */
    if ((n & 1) == 1)  /* n is odd */
        return (seq_t)2;
    uint64_t np = (uint64_t)n + 1;   /* n is even, so np is odd */
    return smallest_prime_factor(np);
}
