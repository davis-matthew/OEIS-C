/*
 * A225490  Expansion of 1/(1 - x - x^2 + x^5 + x^6 - x^7).
 * https://oeis.org/A225490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=7, a(6)=10;
 *            a(n)=a(n-1)+a(n-2)-a(n-5)-a(n-6)+a(n-7) (n≥7)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 64, u64 n <= 130,
 *            u128 n <= 261, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code works for any seq_t and idx_t as specified in the build */
#include <stdlib.h>
#include <stddef.h>

static const seq_t init_seq[7] = {
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3,
    (seq_t)5, (seq_t)7, (seq_t)10
};

static seq_t *cache = NULL;
static idx_t  cache_len = 0;   /* number of elements currently cached */
static size_t cache_cap = 0;   /* capacity of cache (in elements)     */

/* Ensure that the cache can hold at least n+1 elements (indices 0..n)  */
static void ensure_capacity(idx_t n)
{
    if ((idx_t)(cache_cap) >= n + 1)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)(n + 1))
        new_cap <<= 1;

    void *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        abort();                 /* allocation failed */
    }
    cache = (seq_t *)tmp;        /* explicit cast for C++ compatibility */
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* fast path: already cached */
    if (n < cache_len) {
        return cache[n];
    }

    /* extend cache as needed */
    ensure_capacity(n);

    /* compute any missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 7) {
            cache[i] = init_seq[(size_t)i];
        } else {
            /* recurrence: a(i)=a(i-1)+a(i-2)-a(i-5)-a(i-6)+a(i-7)  */
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a5 = cache[i-5];
            seq_t a6 = cache[i-6];
            seq_t a7 = cache[i-7];
            cache[i] = a1 + a2 - a5 - a6 + a7;
        }
    }

    cache_len = n + 1;
    return cache[n];
}
