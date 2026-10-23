/*
 * A160830  Integer part of the product of two consecutive primes divided by
 *          their sum.
 * https://oeis.org/A160830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(prime(n)*prime(n+1)/(prime(n)+prime(n+1)))
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6976, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/* Cache already computed primes */
static uint64_t *primes_cache = NULL;   /* array of primes (uint64_t for safety) */
static size_t primes_cnt   = 0;          /* number of primes stored */
static idx_t   cached_n    = 0;          /* number of indices cached (approach to n+1) */

/* Helper to ensure we have primes up to at least index needed (zero‑based) */
static void ensure_primes_up_to(idx_t need_index)
{
    /* need_index is zero based: need_index = n (for nth prime)  */
    if ((size_t)need_index < primes_cnt)
        return;   /* already have enough */

    idx_t wanted_n    = need_index + 1; /* number of primes needed */
    idx_t limit       = _prime_limit_for_idx(wanted_n);
    /* In rare case the estimate is too low, keep enlarging until we have enough */
    while (1) {
        uint64_t *pr = NULL;
        size_t    cnt = 0;
        build_primes_upto(limit, &pr, &cnt);
        if (cnt >= (size_t)wanted_n) {
            /* replace cache */
            free(primes_cache);
            primes_cache = pr;
            primes_cnt   = cnt;
            cached_n    = wanted_n;
            return;
        }
        free(pr);
        /* increase limit: double it */
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for offset 1, but guard */
        return 0;

    /* We need primes at indices n-1 and n (zero based) */
    ensure_primes_up_to((idx_t)n);
    /* primes_cache now has at least n+1 primes */
    uint64_t p1 = primes_cache[n - 1];
    uint64_t p2 = primes_cache[n];

    seq_t prod = (seq_t)p1 * (seq_t)p2;   /* product, cast to seq_t first */
    seq_t sum  = (seq_t)p1 + (seq_t)p2;

    return prod / sum;   /* integer division truncates towards zero */
}
