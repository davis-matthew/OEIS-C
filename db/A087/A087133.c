/*
 * A087133  Number of divisors of n that are not greater than the greatest
 *          prime-factor of n; a(1)=1.
 * https://oeis.org/A087133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   factor_u64, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculation: a(n) = number of divisors of n that are <= the greatest
 * prime factor of n.  a(1) = 1.  The result is returned as type seq_t. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64() */
#include "../utils/gpf.h"          /* gpf() */

static void dfs_divs(size_t idx, uint64_t current,
                     const uint64_t *pr, const uint64_t *ex,
                     size_t cnt, uint64_t gpf_val, seq_t *res)
{
    if (idx == cnt) {
        if (current <= gpf_val)
            (*res)++;
        return;
    }

    /* Generate divisor factor by factor for the current prime. */
    uint64_t val = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        /* If the current product already exceeds the greatest prime
         * factor, any further exponent will only increase it. */
        if (current * val > gpf_val)
            break;

        dfs_divs(idx + 1, current * val, pr, ex, cnt,
                 gpf_val, res);

        val *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    uint64_t nn = (uint64_t)n;

    /* Factorise nn.  64 is enough for all n ≤ 10 000. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    uint64_t gpf_val = gpf(nn);   /* greatest prime factor of n */

    seq_t result = 0;
    dfs_divs(0, 1, primes, exps, cnt, gpf_val, &result);

    return result;
}
