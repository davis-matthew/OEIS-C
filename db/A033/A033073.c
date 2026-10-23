/*
 * A033073  Numbers with the property that all pairs of consecutive base-8
 *          digits differ by 1.
 * https://oeis.org/A033073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033073: Numbers whose base‑8 digits are stepping numbers
   (adjacent digits differ by 1).  The function builds the sequence
   lazily by breadth‑first expansion of valid digits, storing the
   terms in ascending order in a static cache. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for generated terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t front = 0;       /* next index to expand */

    if (n == 0)   /* undefined, but return 0 for safety */
        return (seq_t)0;

    /* initialise the cache with the 7 single‑digit roots */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();

        for (idx_t d = 1; d <= 7; ++d)    /* 1 … 7 decimal */
            cache[cache_len++] = (seq_t)d;
    }

    /* grow the cache as needed */
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap * 2 + 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* expand nodes until we have at least n terms */
    while (cache_len < n) {
        seq_t x = cache[front++];
        seq_t last = x % (seq_t)8;

        /* child with digit (last-1) if possible */
        if (last > 0) {
            seq_t child = x * (seq_t)8 + (last - (seq_t)1);
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2 + 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = child;
        }

        /* child with digit (last+1) if possible */
        if (last < 7) {
            seq_t child = x * (seq_t)8 + (last + (seq_t)1);
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2 + 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = child;
        }
    }

    return cache[n - 1];
}
