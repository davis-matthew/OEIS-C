/*
 * A086369  Number of factors over Q in the factorization of T_n(x) - 1 where
 *          T_n(x) is the Chebyshev polynomial of the first kind.
 * https://oeis.org/A086369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*τ(n) + (n mod 2) – 2, τ(n) = number of divisors of n
 * Range:     u8 n <= 2049, u16 n <= 2049, u32 n <= 2049, u64 n <= 2049,
 *            u128 n <= 2049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n = 0, return 0 for safety */
        return 0;

    unsigned long long nn = (unsigned long long)n;
    unsigned long long tmp = nn;
    unsigned long long tau = 1;

    for (unsigned long long p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            unsigned long long exp = 0;
            do {
                tmp /= p;
                ++exp;
            } while (tmp % p == 0);
            tau *= (exp + 1);
        }
    }
    if (tmp > 1)                 /* remaining prime factor */
        tau *= 2;

    seq_t result = (seq_t)(2 * tau + (nn & 1ULL) - 2ULL);
    return result;
}
