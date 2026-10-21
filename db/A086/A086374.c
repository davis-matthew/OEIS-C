/*
 * A086374  Number of factors over Q in the factorization of T_n(x) + 1 where
 *          T_n(x) is the Chebyshev polynomial of the first kind.
 * https://oeis.org/A086374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * τ(odd_part(n)) - (n is odd ? 1 : 0) where τ(m) is the
 *            number of positive divisors of m. odd_part(n) = n with all
 *            factors of 2 removed. This formula follows from the
 *            factorization of Chebyshev polynomials: for odd n, T_n(x)+1 =
 *            (x+1) * ∏_{d|n,d>1} f_d(x)^2 and for even n, T_n(x)+1 = ∏_{...
 * Range:     u8 n <= 2049, u16 n <= 2049, u32 n <= 2049, u64 n <= 2049,
 *            u128 n <= 2049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0) return (seq_t)0;

    /* Extract the odd part of n */
    uint64_t m = (uint64_t)n;
    while ((m & 1ULL) == 0ULL) m >>= 1U;

    /* Compute τ(m): number of divisors of m */
    seq_t tau = 1;
    uint64_t tmp = m;
    /* trial division up to sqrt(tmp) */
    for (uint64_t p = 2U; p * p <= tmp; ++p) {
        if (tmp % p == 0U) {
            unsigned exp = 0;
            while (tmp % p == 0U) {
                tmp /= p;
                exp++;
            }
            tau *= (seq_t)(exp + 1);
        }
    }
    if (tmp > 1U) {
        tau *= 2; /* remaining prime factor with exponent 1 */
    }

    /* Assemble the final answer */
    seq_t ans = tau * 2U;
    if ((n & 1ULL) != 0ULL) {
        ans -= 1U;
    }
    return ans;
}
