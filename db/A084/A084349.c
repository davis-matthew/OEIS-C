/*
 * A084349  Squarefree numbers that are not the sum of two squares.
 * https://oeis.org/A084349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A084349: squarefree numbers that contain at least one prime factor ≡ 3 (mod 4).
 * Computation is performed by scanning positive integers in order,
 * testing each for squarefreeness and the presence of a 3‑mod‑4 prime.
 * The first 24 terms are:
 *   1, 3, 6, 7, 11, 14, 15, 19, 21, 22, 23, 30, 31, 33, 35, 38, 39,
 *   42, 43, 46, 47, 51, 55, 57
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* declares factor_u64(uint64_t,uint64_t*,uint64_t*,size_t*) */

static seq_t *cache = NULL;      /* cached terms of the sequence */
static size_t cache_cap = 0;     /* capacity of the cache */
static size_t cache_len = 0;     /* number of terms currently stored */
static seq_t next_cand = 1;      /* next integer to test */

/* Test whether a number is squarefree and has a prime factor ≡ 3 (mod 4). */
static bool _qualifies(seq_t val)
{
    if ((uint64_t)val == 1)            /* 1 counts in this sequence */
        return true;

    uint64_t n = (uint64_t)val;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    /* squarefree check */
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] != 1)
            return false;

    /* at least one prime ≡ 3 (mod 4) ? */
    for (size_t i = 0; i < cnt; ++i)
        if ((primes[i] & 3ULL) == 3ULL)
            return true;

    return false;   /* all prime factors are 1 or 2 (mod 4) */
}

/* Ensure the cache has room for another term. */
static void _grow_cache(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* a(1) is the first term; ignore n == 0 */
    if (n == 0)
        return 0;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (_qualifies(next_cand)) {
            _grow_cache();
            cache[cache_len++] = next_cand;
        }
        ++next_cand;
    }
    return cache[n - 1];
}
