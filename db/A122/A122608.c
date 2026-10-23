/*
 * A122608  a(1) = 1; a(2) = 1; a(3) = 1; a(4) = 1; a(5) = 1; a(n) =
 *          a(n-1)+4a(n-2)-3a(n-3)-3a(n-4)+a(n-5) for n >= 6.
 * https://oeis.org/A122608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1)+4*a(n-2)-3*a(n-3)-3*a(n-4)+a(n-5)
 * with a(1)=a(2)=a(3)=a(4)=a(5)=1
 */
#include <stdlib.h>
#include <stdint.h>

/* The build supplies seq_t and idx_t types. */

static seq_t *cache = NULL;   /* stores computed terms, 0‑based index for a(1) */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_len = 0;  /* number of initialized terms */

/* Ensure the cache has capacity for at least n terms (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= (idx_t)cache_len) return;          /* already have terms up to n */

    /* Allocate/expand the cache */
    if (cache == NULL) {
        cache_cap = (size_t)(n < 5 ? 5 : n);   /* at least 5 terms for init */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize new terms */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        if (i < 5) {                 /* a(1) .. a(5) are 1 */
            cache[i] = 1;
        } else {
            /* recurrence: a(n) = a(n-1)+4*a(n-2)-3*a(n-3)-3*a(n-4)+a(n-5) */
            cache[i] = cache[i-1]
                       + (seq_t)4 * cache[i-2]
                       - (seq_t)3 * cache[i-3]
                       - (seq_t)3 * cache[i-4]
                       + cache[i-5];
        }
    }
    cache_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of bounds, should not happen */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
