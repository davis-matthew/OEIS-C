/*
 * A105824  a(n) = sigma(n) mod 4.
 * https://oeis.org/A105824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n) mod 4
 * Range:     u8 n <= 9900, u16 n <= 9900, u32 n <= 9900, u64 n <= 9900,
 *            u128 n <= 9900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sum of divisors of n and return it modulo 4.
   Uses the multiplicative property of sigma and works for any
   uint64_t n.  No external utilities are required. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* not used, but safe */

    uint64_t m = (uint64_t)n;          /* local copy for factorisation */
    int result = 1 & 3;                /* product of terms modulo 4 */

    /* factor 2 */
    int exp = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++exp;
    }
    if (exp > 0) {
        int term = 0;
        int pow_mod = 1; /* 2^0 == 1 */
        for (int i = 0; i <= exp; ++i) {
            term = (term + pow_mod) & 3;
            pow_mod = (pow_mod * 2) & 3;
        }
        result = (result * term) & 3;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            int term = 0;
            int p_mod = (int)(p % 4);
            int pow_mod = 1;
            for (int i = 0; i <= exp; ++i) {
                term = (term + pow_mod) & 3;
                pow_mod = (pow_mod * p_mod) & 3;
            }
            result = (result * term) & 3;
        }
    }

    /* remaining prime factor > 1 (if any) */
    if (m > 1) {                 /* exponent = 1 */
        int term = (1 + (int)(m % 4)) & 3;
        result = (result * term) & 3;
    }

    return (seq_t)result;
}
