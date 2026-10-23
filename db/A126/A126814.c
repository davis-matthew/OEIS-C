/*
 * A126814  Ramanujan numbers (A000594) read mod 16.
 * https://oeis.org/A126814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 * sigma(n)) mod 16 where sigma(n) = sum of divisors
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t MOD = 16;

    /* Compute n^3 mod 16 using modular arithmetic
       to avoid overflow */
    seq_t nm = (seq_t)n % MOD;
    seq_t n2m = (nm * nm) % MOD;
    seq_t n3m = (n2m * nm) % MOD;

    /* Compute sigma(n) mod 16 by enumerating divisors up to sqrt(n) */
    seq_t sigma_mod = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* Add divisor i */
            sigma_mod += (seq_t)(i % MOD);
            /* Add complementary divisor j = n / i, if distinct */
            idx_t j = n / i;
            if (j != i) {
                sigma_mod += (seq_t)(j % MOD);
            }
        }
    }
    sigma_mod %= MOD;

    /* Final result: (n^3 * sigma(n)) mod 16 */
    seq_t result = (n3m * sigma_mod) % MOD;
    return result;
}
