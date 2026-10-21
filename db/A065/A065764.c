/*
 * A065764  Sum of divisors of square numbers.
 * https://oeis.org/A065764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma(n^2) (sum of the divisors of n squared)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2250, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; n==0 is unused */

    uint64_t x = (uint64_t)n;
    uint64_t primes[20];
    int exps[20];
    int cnt = 0;                           /* number of distinct prime factors */

    /* factor_uint64(x, fac, exp, &cnt) stores the factorisation in fac and exp and the count in cnt */
    factor_uint64(x, primes, exps, &cnt);

    seq_t result = 1;
    for (int i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];

        /* p^(2*e+1) */
        int exp = 2 * e + 1;
        seq_t power = 1;
        for (int j = 0; j < exp; ++j) power *= (seq_t)p;

        /* (p^(2*e+1)-1)/(p-1) */
        seq_t term = (power - 1) / ((seq_t)p - 1);
        result *= term;
    }

    return result;
}
