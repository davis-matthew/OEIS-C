/*
 * A062793  Moebius transform of n^n.
 * https://oeis.org/A062793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * (n/d)^(n/d)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, factor_u64, mu_of_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/mu_of_divisor.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 (won't be used) */
    if (n == 0) return 0;

    /* Factorize n to obtain prime bases and their exponents for divisor generation. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Generate all divisors of n.  For n ≤ 10 000 the divisor count is < 200.  */
    uint64_t divisors[512];
    size_t dcnt = 1;
    divisors[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        int e = (int)exps[i];
        size_t cur = dcnt;
        uint64_t p_pow = 1;
        for (int k = 1; k <= e; ++k) {
            p_pow *= p;
            for (size_t j = 0; j < cur; ++j) {
                divisors[dcnt++] = divisors[j] * p_pow;
            }
        }
    }

    /* Compute the Möbius transform sum. */
    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divisors[i];
        idx_t k = (idx_t)(n / d);            /* n/d fits in idx_t (≤ n) */
        seq_t term = big_pow((seq_t)k, (seq_t)k);   /* (n/d)^(n/d) */
        int mu = mu_of_divisor(d, primes, pcnt);
        if (mu == 1)        sum += term;
        else if (mu == -1)  sum -= term;
        /* mu == 0: skip */
    }
    return sum;
}
