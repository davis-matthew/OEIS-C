/*
 * A075881  a(n) = the sum of the prime factors of Sum_{i=1..n} prime(i).
 * https://oeis.org/A075881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the distinct prime factors of * Sum_{i=1..n}
 *            prime(i) (OEIS A075881)
 * Range:     u8 n <= 13, u16 n <= 157, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static storage for primes and cumulative sums */
    static uint64_t *primes = NULL;   /* array of primes (uint64_t) */
    static size_t primes_cnt = 0;     /* number of primes */
    static uint64_t *cum = NULL;      /* cumulative sums (uint64_t) */
    static size_t cum_cnt = 0;        /* size of cum array */

    /* ensure we have at least n primes */
    if (primes_cnt < (size_t)n) {
        /* free previous data */
        if (primes) free(primes);
        if (cum) free(cum);

        /* find an upper bound for the nth prime */
        idx_t limit = _prime_limit_for_idx(n);

        /* build primes up to the limit */
        build_primes_upto(limit, &primes, &primes_cnt);

        /* allocate cumulative sums */
        cum_cnt = primes_cnt;
        cum = (uint64_t *)malloc(cum_cnt * sizeof(uint64_t));
        uint64_t acc = 0;
        for (size_t i = 0; i < cum_cnt; ++i) {
            acc += (uint64_t)primes[i];
            cum[i] = acc;
        }
    }

    /* sum of the first n primes */
    size_t idx = (size_t)(n - 1);
    uint64_t sum_primes = cum[idx];

    /* sum of distinct prime factors of sum_primes */
    uint64_t val = sum_primes;
    unsigned long long sum_facs = 0;  /* intermediate accumulator */

    /* factor using the primes array */
    for (size_t i = 0; i < primes_cnt && ((uint64_t)primes[i] * (uint64_t)primes[i] <= val); ++i) {
        uint64_t p = primes[i];
        if (val % p == 0) {
            sum_facs += (unsigned long long)p;
            while (val % p == 0) val /= p;
        }
    }
    if (val > 1) sum_facs += (unsigned long long)val;

    return (seq_t)sum_facs;
}
