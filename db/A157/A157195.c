/*
 * A157195  a(n) = 0 if n is 1 or a prime, otherwise a(n) = product of the
 *          proper divisors of n.
 * https://oeis.org/A157195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is 1 or prime; otherwise a(n) is the product of
 *            all proper divisors of n
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Simple prime factorization for n ≤ 10⁴. */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[c]   = p;
            exps[c]     = e;
            ++c;
        }
    }
    if (n > 1) {
        primes[c]   = n;
        exps[c]     = 1;
        ++c;
    }
    *cnt = c;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* 0 for n = 1 or prime. */
    if (n <= 1)
        return 0;

    /* Factor n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;
    factor_uint64((uint64_t)n, primes, exps, &cnt);

    /* If only one prime factor with exponent 1, n is prime. */
    if (cnt == 1 && exps[0] == 1)
        return 0;

    /* Number of divisors d = Π (e_i + 1). */
    uint64_t d = 1;
    for (size_t i = 0; i < cnt; ++i)
        d *= (exps[i] + 1);

    /* Compute product of proper divisors: Π p_i^{ (e_i * (d-2)) / 2 }. */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t exp = exps[i] * (d - 2) / 2;
        seq_t term   = big_pow((seq_t)primes[i], (seq_t)exp);
        result *= term;
    }
    return result;
}
