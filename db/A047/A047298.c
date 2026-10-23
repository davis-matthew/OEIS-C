/*
 * A047298  Numbers that are congruent to {1, 3, 4, 6} mod 7.
 * https://oeis.org/A047298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=3, a(3)=4, a(4)=6, a(5)=8, a(n) =
 *            a(n-1)+a(n-4)-a(n-5) for n>5
 * Range:     u8 n <= 146, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th term of OEIS A047298 (numbers ≡ {1,3,4,6} mod 7) */
#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic cache for computed terms */
static size_t cache_size = 0;    /* number of terms currently cached (>=0) */
static size_t cache_cap  = 0;    /* allocated capacity of the cache */

seq_t a(idx_t n)
{
    /* n must be >= 1; behavior for n < 1 is undefined. */
    if (n == (idx_t)0) return 0;  /* safety fallback */

    /* If we already have the value, return it immediately */
    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1U];

    /* Ensure the cache buffer is large enough */
    if (cache == NULL) {
        cache_cap = 16;                        /* initial capacity */
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;           /* allocation failure */
        cache_size = 0;
    }

    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;  /* double until enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;                     /* realloc failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Fill in any missing terms up to n using the recurrence */
    for (size_t i = cache_size; i < (size_t)n; ++i) {
        if (i < 5U) {                 /* initial values */
            switch (i) {
                case 0: cache[i] = 1; break;
                case 1: cache[i] = 3; break;
                case 2: cache[i] = 4; break;
                case 3: cache[i] = 6; break;
                case 4: cache[i] = 8; break;
            }
        } else {                      /* recurrence: a(n)=a(n-1)+a(n-4)-a(n-5) */
            cache[i] = cache[i-1] + cache[i-4] - cache[i-5];
        }
    }
    cache_size = (size_t)n;

    return cache[(size_t)n - 1U];
}
