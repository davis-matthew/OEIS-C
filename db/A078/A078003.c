/*
 * A078003  Expansion of (1-x)/(1-2*x+2*x^2-2*x^3).
 * https://oeis.org/A078003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3), with a(0)=1, a(1)=1,
 *            a(2)=0
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 54, u64 n <= 105,
 *            u128 n <= 207, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Initialise the cache with the first three terms.  */
static void init_cache(void)
{
    idx_t init_size = 3;
    cache = (seq_t *)malloc(init_size * sizeof(seq_t));
    if (!cache) return;           /* allocation failure – let the call fail */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)0;
    cache_size = init_size;
}

/* Return the n‑th term of A078003.  */
seq_t a(idx_t n)
{
    if (n < 3) {
        /* Small indices are returned directly without caching. */
        if (n == 0) return (seq_t)1;
        if (n == 1) return (seq_t)1;
        /* n==2 */
        return (seq_t)0;
    }

    if (!cache) init_cache();
    if (!cache) return (seq_t)0;  /* failed to allocate */

    /* Grow the cache up to n.  */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        /* Compute missing terms from current size up to n.  */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = cache[i-1] + cache[i-3];     /* a(i-1)+a(i-3) */
            seq_t twice = sum + sum;                 /* 2*(a(i-1)+a(i-3)) */
            seq_t sub = cache[i-2] + cache[i-2];     /* 2*a(i-2) */
            cache[i] = twice - sub;                  /* recurrence value */
        }
        cache_size = new_size;
    }
    return cache[n];
}
