/*
 * A011264  In the prime factorization of n, increment even powers and
 *          decrement odd powers (multiplicative).
 * https://oeis.org/A011264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} p^{ e+1 if e is even, e-1 if e is odd }
 * Range:     u8 n <= 48, u16 n <= 1680, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            unsigned int f = (e & 1) ? e - 1 : e + 1; /* e odd → f=e-1, even → f=e+1 */
            seq_t term = 1;
            for (unsigned int i = 0; i < f; ++i)
                term *= (seq_t)p;
            result *= term;
        }
    }

    /* If a prime > sqrt(n) remains, its exponent is 1 (odd) → f=0, term=1. */
    return result;
}
