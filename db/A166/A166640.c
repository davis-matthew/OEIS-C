/*
 * A166640  Totally multiplicative sequence with a(p) = 9*(p-1) for prime p.
 * https://oeis.org/A166640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 1535, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Formula: a(n) = product_{p^e || n} (9 * (p - 1))^e
 *
 * Here a(1) = 1 and a is multiplicative:
 *   a(p^e) = (9*(p-1))^e,
 *   a(n) = ∏_p (9*(p-1))^{v_p(n)}.
 *
 * The function below factors n, computes the total exponent sum for 9,
 * and multiplies the corresponding powers of (p-1).
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static inline seq_t pow_seq(seq_t base, unsigned int exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1U)
            res *= base;
        base *= base;
        exp >>= 1U;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    uint64_t tn = (uint64_t)n;
    /* maximum number of distinct primes for a 64‑bit integer is < 64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(tn, primes, exps, &cnt);

    /* sum of exponents = bigomega(n) */
    unsigned int sum_exp = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum_exp += (unsigned int)exps[i];

    seq_t result = pow_seq(9, sum_exp);

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t base = (seq_t)(primes[i] - 1);
        result *= pow_seq(base, (unsigned int)exps[i]);
    }

    return result;
}
