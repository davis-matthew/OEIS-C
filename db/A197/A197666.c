/*
 * A197666  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 4,0,1,0,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +4*a(n-2) -25*a(n-4) -37*a(n-5) +4*a(n-6)
 *            +23*a(n-7) +17*a(n-8) -5*a(n-9) -a(n-10); Sequence starts at n =
 *            1. The first ten terms are used as seeds for the recurrence
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t seed[10] = {
    (seq_t)3,   (seq_t)9,   (seq_t)22,  (seq_t)71,
    (seq_t)217, (seq_t)642, (seq_t)1963, (seq_t)5963,
    (seq_t)18018,(seq_t)54753
};

static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static idx_t  cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for n < 1 */

    if (n <= 10) return seed[n-1];

    /* initialise cache with the seed values */
    if (cache == NULL) {
        cache_len = 10;
        cache_cap = 10;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) abort();
        for (idx_t i = 0; i < 10; ++i) cache[i] = seed[i];
    }

    /* grow the cache if needed */
    if (cache_len < n) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;    /* double size until enough */
        if (new_cap != cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute terms from cache_len up to n-1 (0‑based indices) */
        for (idx_t i = cache_len; i < n; ++i) {
            idx_t k = i + 1;          /* current n for the recurrence */

            /* positive part: 3*a(k-1) +4*a(k-2) +4*a(k-6) +23*a(k-7) +17*a(k-8) */
            seq_t pos = 3 * cache[k-2] + 4 * cache[k-3] +
                        4 * cache[k-7] + 23 * cache[k-8] +
                        17 * cache[k-9];

            /* negative part: 25*a(k-4) +37*a(k-5) +5*a(k-9) + a(k-10) */
            seq_t neg = 25 * cache[k-5] + 37 * cache[k-6] +
                        5 * cache[k-10] + cache[k-11];

            cache[i] = pos - neg;   /* rely on unsigned wrap‑around when positive */
        }
        cache_len = n;
    }

    return cache[n-1];
}
