/*
 * A098528  Expansion of (1 + 2*x^3)/(1 - x - 4*x^7).
 * https://oeis.org/A098528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-7) Initial terms: a(0)=1, a(1)=1, a(2)=1,
 *            a(3)=3, a(4)=3, a(5)=3, a(6)=3
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A098528.
   Memoisation is used so that calls are fast and the order of calls
   does not matter.  No external libraries beyond <stdlib.h> are
   required. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache      = NULL; /* array of terms */
    static size_t cache_len  = 0;    /* number of elements stored */
    static size_t cache_cap  = 0;    /* allocated capacity   */

    /* Ensure the cache can hold indices up to n (i.e. at least n+1 elements). */
    size_t required = (size_t)n + 1u;
    if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : required;
        if (new_cap < required) new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first seven terms if the cache has not been seeded. */
    if (cache_len == 0) {
        if (cache_cap < 7) { /* should never happen due to above growth */
            size_t new_cap = 7;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)3;
        cache[5] = (seq_t)3;
        cache[6] = (seq_t)3;
        cache_len = 7;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        /* For i < 7 the values are already set. */
        if (i >= 7) {
            /* a(i) = a(i-1) + 4*a(i-7) */
            seq_t prev   = cache[i - 1];
            seq_t prev7  = cache[i - 7];
            cache[i] = prev + (seq_t)4 * prev7;
        }
        /* else: already initialised above */
    }

    cache_len = n + 1;
    return cache[n];
}
