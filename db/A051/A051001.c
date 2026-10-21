/*
 * A051001  Sum of 4th powers of odd divisors of n.
 * https://oeis.org/A051001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, look
 * Formula:   a(n) = sum_{d|n, d odd} d^4
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: multiplicative formula: for each odd prime factor p^e of n,
   multiply the result by 1 + p^4 + p^8 + … + p^{4e}.  2^k contributes only 1. */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for zero, return 0 as placeholder */

    seq_t result = 1;
    idx_t m = n;

    /* remove factor 2 completely – odd divisors are unaffected */
    while ((m & 1ULL) == 0) {
        m >>= 1;
    }

    /* trial division on the odd part */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p != 0) continue;

        /* count exponent of p */
        int e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* compute term = 1 + p^4 + p^8 + ... + p^{4e} */
        seq_t p4 = (seq_t)p * p * p * p;        /* p^4, fits in seq_t for small p */
        seq_t term = 1;
        seq_t power = 1;
        for (int j = 0; j < e; ++j) {
            power *= p4;
            term += power;
        }
        result *= term;
    }

    /* if something remains, it is a prime factor > sqrt(original n) */
    if (m > 1) {
        /* exponent e = 1 for the remaining prime */
        seq_t p4 = (seq_t)m * m * m * m;
        seq_t term = 1 + p4;   /* 1 + p^4 */
        result *= term;
    }

    return result;
}
