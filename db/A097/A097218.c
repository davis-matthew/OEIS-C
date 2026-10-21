/*
 * A097218  Numbers n such that A076078(n) < n, where A076078(n) equals the
 *          number of sets of distinct positive integers with a least common
 *          multiple of n.
 * https://oeis.org/A097218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer n for which A076078(n) < n (computed by
 *            searching)
 * Range:     u8 n <= 46, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

static bool cond(idx_t n)
{
    if (n <= 1)   /* 1 never satisfies A076078(n) < n */
        return false;

    /* factor n: primes[i]^exps[i] */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t total = 0;
    uint64_t mask_limit = 1ULL << cnt;   /* cnt <= 64 */

    for (uint64_t mask = 0; mask < mask_limit; ++mask) {
        uint64_t prod = 1;
        for (size_t i = 0; i < cnt; ++i) {
            if (mask & (1ULL << i))
                prod *= (uint64_t)exps[i];         /* exponent limited to < e */
            else
                prod *= (uint64_t)(exps[i] + 1);    /* exponent may be e */
        }

        seq_t term = seq_pow((seq_t)2, (idx_t)prod);    /* 2^prod */
        if (__builtin_popcountll(mask) % 2 == 0)
            total += term;
        else
            total -= term;
    }

    return total < (seq_t)n;
}

/* ---------- cache of found terms ---------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void cache_append(uint64_t value)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t)value;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;         /* offset is 1 */

    /* if requested term is already cached */
    if (cache_len >= n)
        return cache[n - 1];

    /* search for new terms */
    uint64_t candidate = (cache_len == 0) ? 2 : (uint64_t)cache[cache_len - 1] + 1;

    while (cache_len < n) {
        if (cond(candidate))
            cache_append(candidate);
        ++candidate;
    }

    return cache[n - 1];
}
