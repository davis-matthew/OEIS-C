/*
 * A094196  Indices of the start of a string of 24 consecutive squares whose
 *          sum is a square.
 * https://oeis.org/A094196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 52, u64 n <= 106, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = 10 a(n-6) – a(n-12) + 92  for n > 12
    (initial values a(1)…a(12) are used directly) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Array of the first 12 terms */
    static const seq_t init[12] = {
        (seq_t)1,  (seq_t)9,  (seq_t)20, (seq_t)25,
        (seq_t)44, (seq_t)76, (seq_t)121, (seq_t)197,
        (seq_t)304,(seq_t)353,(seq_t)540,(seq_t)856
    };

    /* Run‑time cache of all computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* capacity in cache_len units */

    /* index in 0‑based array (a(1) at index 0) */
    size_t idx = (size_t)(n - 1);

    /* return immediately for the first 12 terms */
    if (idx < 12)
        return init[idx];

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 12;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failure */
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cache_len = 12;
    }

    /* extend the cache until it covers the requested index */
    while (cache_len <= idx) {
        if (cache_len == cache_cap) {        /* grow by doubling */
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();        /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* a(n) = 10 a(n-6) – a(n-12) + 92 */
        seq_t term = (seq_t)10 * cache[cache_len - 6];
        term -= cache[cache_len - 12];
        term += (seq_t)92;
        cache[cache_len++] = term;
    }

    return cache[idx];
}
