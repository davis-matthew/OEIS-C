/*
 * A225499  Expansion of 1/(1 - x + x^2 - x^3 - x^6 - x^9 + x^10 - x^11 +
 *          x^12).
 * https://oeis.org/A225499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-6) + a(n-9) - a(n-10) +
 *            a(n-11) - a(n-12) Initial values: a(0)=1, a(1)=1, a(2)=0,
 *            a(3)=0, a(4)=1, a(5)=1, a(6)=1, a(7)=2, a(8)=2, a(9)=2, a(10)=3,
 *            a(11)=4, a(12)=4
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 107, u64 n <= 210,
 *            u128 n <= 416, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array holding computed terms */
static idx_t  cache_capacity = 0;    /* current allocated size of the array */

/* Ensure the cache can hold at least [size] elements.  */
static void ensure_cache_size(idx_t size)
{
#if defined(__cplusplus)
    if (!cache) {
        cache = new seq_t[size];
        cache_capacity = size;
        return;
    }
    if (cache_capacity < size) {
        idx_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
        while (new_cap < size) new_cap *= 2;
        seq_t *tmp = new seq_t[new_cap];
        for (idx_t i = 0; i < cache_capacity; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
        cache_capacity = new_cap;
    }
#else
    if (!cache) {
        cache = (seq_t*)malloc(size * sizeof(seq_t));
        cache_capacity = size;
        return;
    }
    if (cache_capacity < size) {
        idx_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
        while (new_cap < size) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_capacity = new_cap;
    }
#endif
}

/* Initialize the cache with the first 13 terms of the sequence. */
static void init_cache(void)
{
    ensure_cache_size(13);
    cache[0] = 1;  /* a(0) */
    cache[1] = 1;  /* a(1) */
    cache[2] = 0;  /* a(2) */
    cache[3] = 0;  /* a(3) */
    cache[4] = 1;  /* a(4) */
    cache[5] = 1;  /* a(5) */
    cache[6] = 1;  /* a(6) */
    cache[7] = 2;  /* a(7) */
    cache[8] = 2;  /* a(8) */
    cache[9] = 2;  /* a(9) */
    cache[10] = 3; /* a(10) */
    cache[11] = 4; /* a(11) */
    cache[12] = 4; /* a(12) */
}

/* Compute the sequence values up to n if necessary and return a(n). */
seq_t a(idx_t n)
{
    /* Initialize if this is the first call. */
    if (cache == NULL) init_cache();

    /* If n is already cached, return it immediately. */
    if (n < 13) return cache[n];

    /* Expand the cache if needed. */
    if (cache_capacity <= n) ensure_cache_size(n + 1);

    /* Compute any terms that are missing up to n. */
    for (idx_t i = 13; i <= n; ++i) {
        cache[i] = cache[i-1] - cache[i-2] + cache[i-3]
                 + cache[i-6] + cache[i-9] - cache[i-10]
                 + cache[i-11] - cache[i-12];
    }

    return cache[n];
}
