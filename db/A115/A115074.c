/*
 * A115074  a(n) is the largest prime dividing the n-th nonsquarefree positive
 *          integer.
 * https://oeis.org/A115074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 402, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  OEIS A115074: a(n) = the largest prime dividing the n‑th nonsquarefree
 *  positive integer.
 *  The sequence is computed by iterating through the natural numbers,
 *  skipping squarefree numbers, and recording the maximum prime factor of
 *  each nonsquarefree number.  Results are cached for repeated calls.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* cache of already computed a(n) values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* next integer to examine (always > the last examined number) */
static uint64_t next_candidate = 2;

/* grow cache if necessary */
static void ensure_cache_capacity(void)
{
    if (cache_len < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8U;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);
    cache = new_ptr;
    cache_cap = new_cap;
}

/* return true if n has a repeated prime factor (i.e. is not squarefree),
   and set maxp to the largest prime factor of n. */
static bool nonsquarefree_and_maxp(uint64_t n, uint64_t *maxp)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    bool non_sq = false;
    uint64_t largest = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] > 1) non_sq = true;
        if (pr[i] > largest) largest = pr[i];
    }
    *maxp = largest;
    return non_sq;
}

/*  a(n) implementation  */
seq_t a(idx_t n)
{
    /*  n >= 1  */
    while (cache_len < (size_t)n) {
        /* find next nonsquarefree integer */
        while (true) {
            uint64_t largest_prime = 0;
            if (nonsquarefree_and_maxp(next_candidate, &largest_prime)) {
                /* store result */
                ensure_cache_capacity();
                cache[cache_len++] = (seq_t)largest_prime;
                ++next_candidate;   /* move on for next search */
                break;
            }
            ++next_candidate;
        }
    }
    return cache[(size_t)n - 1];
}
