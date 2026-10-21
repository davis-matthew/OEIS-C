/*
 * A075802  Characteristic function of perfect powers, A001597.
 * https://oeis.org/A075802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a perfect power (including 1), 0 otherwise The
 *            method factors n by trial division and checks that the gcd of
 *            the prime exponents is at least 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd64.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;          /* 1 is treated as a perfect power */

    uint64_t tmp = (uint64_t)n;
    uint64_t g = 0;                       /* gcd of prime exponents */

    /* factor 2 */
    uint64_t exp = 0;
    while ((tmp & 1ULL) == 0) {           /* tmp % 2 == 0 */
        tmp >>= 1;
        ++exp;
    }
    if (exp) {
        g = exp;
        if (g == 1) return (seq_t)0;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (g == 0) g = exp;
            else g = gcd64(g, exp);
            if (g == 1) return (seq_t)0;
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1) {                        /* exponent 1 */
        if (g == 0) g = 1;
        else g = gcd64(g, 1);
        if (g == 1) return (seq_t)0;
    }

    return (g >= 2) ? (seq_t)1 : (seq_t)0;
}
