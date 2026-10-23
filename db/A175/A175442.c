/*
 * A175442  a(n)>a(n-1), a(n) = smallest prime such that a(n)+a(n-1) is
 *          multiple of m, a(1)=2, m=3.
 * https://oeis.org/A175442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(1)=2; for n>1 a(n) = smallest prime > a(n-1)
   such that (a(n)+a(n-1)) % 3 == 0 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n) */

static seq_t *cache = NULL;       /* cache[0] = a(1) */
static size_t cache_len = 0, cache_cap = 0;

/* Grow the cache so that it has at least `new_cap` elements. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Return the nth term (n>=1). */
seq_t a(idx_t n)
{
    if (n == 0)               /* behaviour for n<1 is irrelevant */
        return (seq_t)0;

    /* Initialise cache on first call. */
    if (!cache) {
        grow_cache();
        cache[0] = (seq_t)2;   /* a(1) */
        cache_len = 1;
    }

    /* Extend cache up to n. */
    while (cache_len < (size_t)n) {
        seq_t const prev = cache[cache_len - 1];   /* a(k-1), where k = cache_len+1 */

        /* Find the remainder that a(k) must have to make the sum divisible by 3. */
        unsigned int prev_mod   = (unsigned int)(prev % (seq_t)3);          /* 0,1,2 */
        unsigned int target_mod = (3 - prev_mod) % 3;                      /* 0,1,2 */

        /* Candidate just greater than prev with correct remainder. */
        seq_t cand = prev + (seq_t)1;
        unsigned int cand_mod = (unsigned int)(cand % (seq_t)3);
        unsigned int shift = (target_mod + 3 - cand_mod) % 3;
        if (shift != 0)
            cand += (seq_t)shift;

        /* Search for the next prime with the required remainder. */
        while (!is_prime_big(cand)) {
            cand += (seq_t)3;   /* keep the same modulo 3 remainder */
        }

        /* Store the new term, growing the array if necessary. */
        if (cache_len == cache_cap)
            grow_cache();
        cache[cache_len++] = cand;
    }

    return cache[n - 1];
}
