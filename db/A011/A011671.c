/*
 * A011671  A binary m-sequence: expansion of reciprocal of x^6+x^5+x^4+x^2+1.
 * https://oeis.org/A011671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) XOR a(n-4) XOR a(n-5) XOR a(n-6) with initial
 *            values a(0)=a(1)=a(2)=a(3)=a(4)=0 and a(5)=1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* allocated cache array   */
static size_t cache_cap = 0;         /* capacity of the array   */
static size_t cache_len = 0;         /* number of entries filled */
static const seq_t init_vals[6] = {0,0,0,0,0,1};

/* Fill the cache up to at least n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < (idx_t)cache_len) return;            /* already computed */

    /* Ensure sufficient capacity. */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return;                       /* allocation failed – caller handles it */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first six terms if not already done. */
    if (cache_len < 6) {
        for (size_t i = cache_len; i < 6 && i < cache_cap; ++i)
            cache[i] = init_vals[i];
        cache_len = 6;
    }

    /* Compute the missing terms using the recurrence. */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i < 6) continue;                    /* safety – first six already set */
        seq_t v = cache[i-2] ^ cache[i-4] ^ cache[i-5] ^ cache[i-6];
        cache[i] = v & 1;                        /* keep only the lowest bit */
    }

    /* Update the number of computed entries. */
    if ((size_t)n + 1 > cache_len) cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
