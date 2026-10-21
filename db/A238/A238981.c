/*
 * A238981  Sum of n-th powers of unitary divisors of n (d such that d divides
 *          n, gcd(d, n/d) = 1).
 * https://oeis.org/A238981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n, gcd(d,n/d)=1} d^n = product_{p^e∥n} (1 +
 *            p^{e*n})
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Empty factorization for n = 1 gives result 1 automatically */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base = (seq_t)primes[i];
        seq_t exp  = (seq_t)exps[i] * (seq_t)n;   /* e * n */
        seq_t pw   = big_pow(base, exp);
        seq_t term = pw + (seq_t)1;              /* 1 + p^{e*n} */
        result = result * term;
    }
    return result;
}
