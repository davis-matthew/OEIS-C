/*
 * A209804  a(n) = Sum_{d|n} d*3^(n*d).
 * https://oeis.org/A209804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * 3^(n*d) (OEIS A209804)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   factor_u64, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
/* utility functions */
#include "../utils/pow3.h"           /* seq_t pow3(idx_t exp); */
#include "../utils/factor_u64.h"     /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

seq_t a(idx_t n)
{
    if (n == 0) /* not used, but safe */
        return 0;

    /* 1. Factor n */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t nfactors = 0;
    factor_u64((uint64_t)n, primes, exps, &nfactors);

    /* 2. Generate all divisors of n */
    uint64_t divs[256];
    size_t divs_len = 0;
    divs[divs_len++] = 1;          /* start with divisor 1 */

    for (size_t i = 0; i < nfactors; ++i) {
        uint64_t p = primes[i];
        uint64_t pow = 1;
        size_t old_len = divs_len;

        for (uint64_t exp = 1; exp <= exps[i]; ++exp) {
            pow *= p;              /* p^exp */

            for (size_t j = 0; j < old_len; ++j) {
                divs[divs_len++] = divs[j] * pow;
            }
        }
    }

    /* 3. Compute result */
    seq_t sum = 0;
    for (size_t i = 0; i < divs_len; ++i) {
        uint64_t d = divs[i];
        seq_t term = (seq_t)d * pow3((idx_t)(n * d));
        sum += term;
    }

    return sum;
}
