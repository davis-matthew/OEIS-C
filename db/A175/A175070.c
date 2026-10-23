/*
 * A175070  a(n) is the sum of perfect divisors of n - n, where a perfect
 *          divisor of n is a divisor d such that d^k = n for some k >= 1.
 * https://oeis.org/A175070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = sum of proper perfect divisors of n, i.e. for each k≥2
 *            dividing the gcd of prime exponents of n add n^(1/k)
 * Range:     u8 n <= 9113, u16 n <= 9113, u32 n <= 9113, u64 n <= 9113,
 *            u128 n <= 9113, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/pow_uint.h"    /* pow_uint */

static idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    /* Factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute gcd of exponents */
    idx_t g = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (i == 0)
            g = exps[i];
        else
            g = gcd_u64(g, exps[i]);
    }

    if (g < 2) return (seq_t)0;   /* no perfect divisor except n itself */

    seq_t sum = 0;

    /* Enumerate all k dividing g, k>=2 */
    for (idx_t k = 2; k <= g; ++k) {
        if (g % k == 0) {
            seq_t d = 1;
            for (size_t i = 0; i < cnt; ++i) {
                idx_t e = exps[i] / k;
                d *= pow_uint(primes[i], e);
            }
            sum += d;
        }
    }

    return sum;
}
