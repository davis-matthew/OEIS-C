/*
 * A017893  Expansion of 1/(1-x^10-x^11-x^12-x^13-x^14-x^15-x^16-x^17).
 * https://oeis.org/A017893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-10)+a(n-11)+a(n-12)+a(n-13)+a(n-14)+a(n-15)+a(n-16)+a(n-17);
 *            Initial values: a(0)=1, a(1..9)=0, a(10..17)=1
 * Range:     u8 n <= 50, u16 n <= 85, u32 n <= 155, u64 n <= 295,
 *            u128 n <= 574, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Allocate or grow the memoization array to hold up to index n. */
    if (cache == NULL) {
        /* Start with space for the first 18 values (0..17). */
        size_t init_size = 18;
        if (n + 1 > init_size)
            init_size = (size_t)n + 1;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) return 0;          /* Allocation failure, return 0. */
        /* Initialise base cases. */
        cache[0] = 1;                  /* a(0) = 1 */
        for (size_t i = 1; i <= 9; ++i) cache[i] = 0;                   /* 1..9 -> 0 */
        for (size_t i = 10; i <= 17; ++i) cache[i] = 1;                  /* 10..17 -> 1 */
        cache_size = init_size;
    } else if ((idx_t)(cache_size) <= n) {
        /* Grow cache to at least n+1 entries. */
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* Allocation failure, return 0. */
        cache = tmp;
        /* Compute newly added entries. */
        for (size_t i = cache_size; i < new_size; ++i) {
            seq_t val = 0;
            for (int k = 10; k <= 17; ++k) {
                if (i >= (size_t)k)
                    val += cache[i - k];
            }
            cache[i] = val;
        }
        cache_size = new_size;
    }

    /* If n is still beyond current cache (possible when n<18 but cache_size>n) */
    if (n >= cache_size) {
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t val = 0;
            for (int k = 10; k <= 17; ++k) {
                if (i >= (size_t)k)
                    val += cache[i - k];
            }
            if (i < cache_size) cache[i] = val;
            else {
                /* This case shouldn't occur due to earlier growth logic. */
                cache[i] = val;
            }
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
