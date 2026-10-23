/*
 * A175865  Numbers k with property that 2^(k-1) == 1 (mod k) and
 *          2^((3*k-1)/2) - 2^((k-1)/2) + 1 == 0 (mod k).
 * https://oeis.org/A175865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 3281, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175865:
 *   a(n) = the nth integer k > 0 such that
 *       2^(k-1) ≡ 1 (mod k)  and
 *       2^((3k-1)/2) – 2^((k-1)/2) + 1 ≡ 0 (mod k).
 *
 *   The function uses a lazy sequential search and caches all found
 *   terms in a static array so that calls in arbitrary order are
 *   handled correctly.  The implementation is fully C11 compatible
 *   and also compiles under C++17.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"   /* mod_pow(seq_t base, uint64_t exp, seq_t mod) */

static seq_t *cache = NULL;          /* array of discovered terms */
static idx_t cache_cap = 0;          /* capacity of the array            */
static idx_t cache_len = 0;          /* current number of stored terms   */
static idx_t tested_up_to = 0;       /* last integer that was tested     */

/* enlarge the cache when needed */
static void ensure_cache_cap(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    /* cast is required for C++ */
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* generate terms until we have at least 'target' terms in cache */
static void generate_to(idx_t target)
{
    while (cache_len < target) {
        ++tested_up_to;                     /* next candidate */

        /* k must be odd – the first congruence cannot hold for even k */
        if ((tested_up_to & 1) == 0)
            continue;

        seq_t k = tested_up_to;

        /* condition 1: 2^(k-1) ≡ 1 (mod k) */
        if (mod_pow(2, tested_up_to - 1, k) != 1)
            continue;

        /* compute the second condition carefully */
        seq_t big  = mod_pow(2, (3 * tested_up_to - 1) / 2, k);
        seq_t small = mod_pow(2, (tested_up_to - 1) / 2, k);

        seq_t diff = big;
        if (diff >= small)
            diff = diff - small;
        else
            diff = diff + k - small;
        diff = diff + 1;
        if (diff >= k)
            diff -= k;   /* avoid modulo */
        if (diff == 0) {
            /* store the qualifying k */
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = k;
        }
    }
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n < 1)        /* undefined for n==0, returns 0 */
        return 0;

    generate_to(n);
    return cache[n - 1];
}
