/*
 * A119663  Triangular numbers with at most two distinct prime factors.
 * https://oeis.org/A119663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth triangular number that has at most two * distinct
 *            prime factors (order by increasing value, offset 1)
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"

/* ---------- internal data structures ---------- */
static seq_t *cache = NULL;      /* stored a(k) values */
static size_t cache_len = 0;     /* number of cached values   */
static size_t cache_cap = 0;     /* allocation capacity       */

/* next triangular index to test (starts at 1) */
static idx_t next_m = 1;

/* ---------- helper functions ---------- */

/* push a new value into the cache, growing if necessary */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();           /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* count the distinct prime factors of a 64‑bit integer x
 * (returns 0 for x==1).  It uses the provided factor_u64 helper. */
static unsigned count_distinct_primes(uint64_t x)
{
    if (x < 2) return 0;
    size_t factor_cnt = 0;
    uint64_t primes[64], exps[64];    /* ample for 64‑bit numbers */
    factor_u64(x, primes, exps, &factor_cnt);
    return (unsigned)factor_cnt;
}

/* ---------- sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset undefined */

    /* return cached value if already computed */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* extend cache until we have the requested n‑th term */
    while (cache_len < (size_t)n) {
        /* triangular number T = m*(m+1)/2 */
        seq_t T = (seq_t)next_m * ((seq_t)next_m + 1) / 2;

        /* factor m and m+1, dividing by 2 only once */
        uint64_t a_part = next_m;
        uint64_t b_part = next_m + 1;
        if ((a_part & 1ULL) == 0) a_part >>= 1;
        else                      b_part >>= 1;

        unsigned cnt = count_distinct_primes(a_part) +
                       count_distinct_primes(b_part);

        if (cnt <= 2) cache_push(T);

        ++next_m;
    }

    return cache[n - 1];
}
