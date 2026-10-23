/*
 * A143265  a(n) = the smallest integer >= n such that all the distinct primes
 *          that divide n and a(n) together are members of a set of
 *          consecutive primes. In other words, a(n) is the smallest integer
 *          >= n such that n*a(n) is contained in sequence A073491.
 * https://oeis.org/A143265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A137795(n) * ceil(n / A137795(n)) where A137795(n) is the
 *            product of the primes between the smallest and largest prime
 *            factor of n that are not divisors of n
 * Range:     u8 n <= 25, u16 n <= 37, u32 n <= 73, u64 n <= 121,
 *            u128 n <= 213, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"          /* factor_u64() */
#include "../utils/build_primes_upto.h"   /* build_primes_upto() */

seq_t a(idx_t n)
{
    /* offset is 1: a(1) = 1 */
    if (n == 1) return (seq_t)1;

    /* Factor n */
    uint64_t pr_f[16];
    uint64_t ex_f[16];
    size_t cnt_f = 0;
    factor_u64((uint64_t)n, pr_f, ex_f, &cnt_f);

    /* Determine min and max prime factor of n */
    uint64_t p_min = pr_f[0];
    uint64_t p_max = pr_f[cnt_f - 1];

    /* Get all primes up to p_max */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(p_max, &primes, &primes_cnt);

    /* Compute A137795(n) : product of missing primes */
    seq_t base = (seq_t)1;
    size_t fi = 0;                /* index in factor list */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p < p_min) continue;
        if (p > p_max) break;

        while (fi < cnt_f && pr_f[fi] < p) ++fi;
        bool present = (fi < cnt_f && pr_f[fi] == p);

        if (!present) {
            base = base * (seq_t)p;
        }
    }

    /* Free the primes array allocated by build_primes_upto */
    free(primes);

    /* Compute ceil(n / base) */
    seq_t N = (seq_t)n;
    seq_t q = N / base;
    seq_t r = N % base;
    if (r != 0) ++q;

    /* Return a(n) = base * ceil(n / base) */
    return q * base;
}
