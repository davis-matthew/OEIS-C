/*
 * A069088  a(n) = Sum_{d|n} core(d) where d are the divisors of n and where
 *          core(d) is the squarefree part of d: the smallest number such that
 *          d*core(d) is a square.
 * https://oeis.org/A069088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = Sum_{d|n} core(d) where core(d) is the product of primes
 *            in d having odd exponent
 * Range:     u8 n <= 137, u16 n <= 9346, u32 n <= 9346, u64 n <= 9346,
 *            u128 n <= 9346, big any n
 * Helpers:   prime_power_contribution
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prime_power_contribution.h"

seq_t a(idx_t n)
{
    if (n < 1)  /* undefined for n < 1, return 0 as a safe default */
        return (seq_t)0;

    uint64_t val = (uint64_t)n;
    if (val == 1)          /* only divisor 1, core(1)=1 */
        return (seq_t)1;

    seq_t result = (seq_t)1;

    /* factor 2 separately */
    if ((val & 1ULL) == 0) {
        idx_t e = 0;
        while ((val & 1ULL) == 0) {
            val >>= 1;
            e++;
        }
        result *= prime_power_contribution(2ULL, e);
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= val; p += 2) {
        if (val % p == 0) {
            idx_t e = 0;
            while (val % p == 0) {
                val /= p;
                e++;
            }
            result *= prime_power_contribution(p, e);
        }
    }

    /* remaining prime factor, if any */
    if (val > 1)
        result *= prime_power_contribution(val, 1);

    return result;
}
