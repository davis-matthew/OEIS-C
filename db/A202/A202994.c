/*
 * A202994  a(n) = sigma(n^4).
 * https://oeis.org/A202994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = σ(n⁴) (sum of divisors of n⁴)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 196, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* offset starts at 1 */

    uint64_t val = (uint64_t)n;

    /* Factorise val.  factor_u64 writes at most 64 prime factors. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(val, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* n⁴ contributes p^(4e).  σ(p^(4e)) = (p^(4e+1)-1)/(p-1). */
        uint64_t exp = 4 * e + 1;
        seq_t pow_val = seq_pow(p, (idx_t)exp);
        seq_t term = (pow_val - 1) / ((seq_t)primes[i] - 1);
        result *= term;
    }

    return result;
}
