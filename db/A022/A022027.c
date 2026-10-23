/*
 * A022027  Define the sequence T(a(0),a(1)) by a(n+2) is the greatest integer
 *          such that a(n+2)/a(n+1) < a(n+1)/a(n) for n >= 0. This is T(2,16).
 * https://oeis.org/A022027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 4*a(n-3), a(0)=2, a(1)=16, a(2)=127
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helpers to manage a dynamic array of seq_t */
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Forward declaration of the cache‑updating helper */
static void ensure_cache(idx_t n, seq_t **cache_ptr,
                         size_t *cap_ptr, size_t *len_ptr);

/* Sequence generator */
seq_t a(idx_t n)
{
    /* Static cache reused across calls */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    ensure_cache(n, &cache, &cache_cap, &cache_len);
    return cache[(size_t)n];
}

/* ------------------------------------------------------------------ */
/* Ensure that the cache contains a[0] … a[n] (and essential base terms) */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n, seq_t **cache_ptr,
                         size_t *cap_ptr, size_t *len_ptr)
{
    /* Minimum capacity required: max(n+1, 3) */
    size_t need = (size_t)(n + 1);
    if (need < 3u) need = 3u;

    /* Grow the underlying array if needed */
    if (*cap_ptr < need) {
        size_t new_cap = (*cap_ptr == 0) ? 1 : *cap_ptr;
        while (new_cap < need) new_cap <<= 1;
        if (*cap_ptr == 0) {
            *cache_ptr = seq_alloc(new_cap);
        } else {
            *cache_ptr = seq_realloc(*cache_ptr, *cap_ptr, new_cap);
        }
        *cap_ptr = new_cap;
    }

    /* Initialise missing base terms (if any) */
    if (*len_ptr == 0) {
        (*cache_ptr)[0] = (seq_t)2;
        (*len_ptr) = 1;
    }
    if (*len_ptr == 1) {
        (*cache_ptr)[1] = (seq_t)16;
        (*len_ptr) = 2;
    }
    if (*len_ptr == 2) {
        (*cache_ptr)[2] = (seq_t)127;
        (*len_ptr) = 3;
    }

    /* Compute any terms up to n (if needed) */
    if (n + 1 > *len_ptr) {
        for (idx_t i = *len_ptr; i <= n; ++i) {
            /* a(i) = 8*a(i-1) - 4*a(i-3) */
            (*cache_ptr)[(size_t)i] =
                (*cache_ptr)[(size_t)(i - 1)] * (seq_t)8
                - (*cache_ptr)[(size_t)(i - 3)] * (seq_t)4;
        }
        *len_ptr = (size_t)(n + 1);
    }
}
