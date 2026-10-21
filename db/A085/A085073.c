/*
 * A085073  Smallest k such that n+k and n*k have the same prime signature, or
 *          0 if no such number exists.
 * https://oeis.org/A085073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k>0 such that prime signature of n*k equals the
 *            prime signature of n+k. Return 0 if no such k exists. The
 *            signature is the multiset of exponents in the prime
 *            factorisation, regardless of which primes occur
 * Range:     u8 n <= 11, u16 n <= 179, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   _factor_u64, combine_exponents
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_factor_u64.h" /* for _factor_u64 */
#include "../utils/combine_exponents.h"

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* factor n once */
    uint64_t pr_n[64], exp_n[64];
    size_t cn;
    _factor_u64((uint64_t)n, pr_n, exp_n, &cn);

    /* buffers for factorising k and n+k */
    uint64_t pr_k[64], exp_k[64];
    uint64_t pr_sum[64], exp_sum[64];
    uint64_t exps_prod[128];

    /* search for smallest k */
    for (uint64_t k = 1;; ++k) {
        uint64_t np = (uint64_t)n + k;

        /* factor n+k */
        size_t cs;
        _factor_u64(np, pr_sum, exp_sum, &cs);

        /* factor k */
        size_t ck;
        _factor_u64(k, pr_k, exp_k, &ck);

        /* exponent multiset of n*k */
        size_t cp = combine_exponents(pr_n, exp_n, cn, pr_k, exp_k, ck,
                                      exps_prod);

        /* sort both exponent lists */
        qsort(exp_sum, cs, sizeof(uint64_t), cmp_uint64);
        qsort(exps_prod, cp, sizeof(uint64_t), cmp_uint64);

        /* compare */
        if (cp == cs) {
            bool equal = true;
            for (size_t i = 0; i < cp; ++i) {
                if (exps_prod[i] != exp_sum[i]) {
                    equal = false;
                    break;
                }
            }
            if (equal)
                return (seq_t)k;
        }

        /* safeguard against infinite loops */
        /* the sequence data suggests k rarely exceeds a few million for n≤10000 */
        if (k > 10000000ULL)
            break;
    }

    return (seq_t)0;
}
