/*
 * A034678  Sum of fourth powers of unitary divisors.
 * https://oeis.org/A034678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, changed
 * Formula:   a(n) = ∏_{p^k‖n} (1 + p^(4k)) (sum of fourth powers of unitary
 *            divisors)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 253, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* offset starts at 1 */

    uint64_t num = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(num, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t base = (seq_t)primes[i];
        idx_t  exp4 = (idx_t)(4 * exps[i]);   /* 4 * exponent */
        seq_t p_pow = big_pow(base, (seq_t)exp4);
        seq_t term  = p_pow + (seq_t)1;
        result = result * term;
    }

    return result;
}
