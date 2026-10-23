/*
 * A048746  Partial sums of A048655.
 * https://oeis.org/A048746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 4; a(0)=1, a(1)=6
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached sequence storage */
static seq_t *a_cache   = NULL;      /* pointer to stored terms */
static size_t cache_cap = 0;          /* allocated capacity */
static size_t cache_len = 0;          /* number of terms initialized */

/* Ensure a_cache[0 .. n] is defined */
static void ensure_cache(idx_t n)
{
    /* First allocation */
    if (a_cache == NULL) {
        cache_cap = 2;
        a_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!a_cache) exit(EXIT_FAILURE);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)6;
        cache_len = 2;
    }

    /* Grow capacity if required */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_cache   = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        a_cache[i] = (seq_t)2 * a_cache[i - 1] + a_cache[i - 2] + (seq_t)4;
    }

    if ((size_t)n >= cache_len) cache_len = (size_t)n + 1;
}

/* Sequence function requested by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[(size_t)n];
}
