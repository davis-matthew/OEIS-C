/*
 * A076570  Greatest prime divisor of sum of first n primes.
 * https://oeis.org/A076570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = greatest prime divisor of sum of the first n primes
 * Range:     u8 n <= 13, u16 n <= 157, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9054 (2026-11-03)
 * Helpers:   build_primes_upto, largest_prime_factor, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stddef.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/largest_prime_factor.h"
#include "../utils/mem.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    /* Cache of primes and their running sums. */
    static uint64_t *primes   = NULL;      /* first primes, sorted */
    static size_t   primes_cnt = 0;        /* number of cached primes */
    static seq_t *prefix        = NULL;    /* prefix sums: prefix[i] = sum of first i primes */
    static size_t   prefix_len  = 0;        /* length of prefix array (primes_cnt + 1) */

    if (n == 0)
        return 0;                         /* undefined for n = 0; fallback */

    /* Build or enlarge the cache if it currently contains fewer than n primes. */
    if (primes == NULL || primes_cnt < (size_t)n) {
        /* free existing cache */
        if (primes)  free(primes);
        if (prefix)  free_seq_array(prefix);

        /* estimate an upper bound for the nth prime and keep doubling until we have enough. */
        uint64_t limit = _prime_limit_for_idx(n);   /* known safe upper bound */
        size_t   count = 0;
        uint64_t *tmp  = NULL;

        while (1) {
            build_primes_upto(limit, &tmp, &count);
            if (count >= (size_t)n)
                break;
            free(tmp);
            limit *= 2;
        }

        primes    = tmp;
        primes_cnt = count;

        /* Allocate the prefix sum array (type seq_t to support arbitrary precision). */
        prefix_len = primes_cnt + 1;
        prefix     = alloc_seq_array(prefix_len);

        /* Compute prefix sums. */
        prefix[0] = (seq_t)0;
        seq_t acc = (seq_t)0;

        for (size_t i = 0; i < primes_cnt; ++i) {
            acc += (seq_t)primes[i];
            prefix[i + 1] = acc;
        }
    }

    /* Sum of the first n primes. */
    seq_t sum = prefix[(size_t)n];

    /* The sum fits in uint64_t for the range we need to support (n ≤ 10000),
       so we can safely cast it for the prime‑factor function. */
    uint64_t sum64 = (uint64_t)sum;
    uint64_t lpf64 = largest_prime_factor(sum64);

    return (seq_t)lpf64;
}
