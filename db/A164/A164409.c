/*
 * A164409  Number of binary strings of length n with no substrings equal to
 *          0000 0001 or 0100.
 * https://oeis.org/A164409
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 78, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A164409: Number of binary strings of length n with no substrings
 * 0000, 0001, or 0100.
 * 
 * Recurrence: a(4)=13, a(5)=22, a(6)=39,
 *              a(n) = 2*a(n-1) - a(n-2) + a(n-3)   for n >= 7
 */

#include <stdlib.h>
#include <stdint.h>

/* The function signature required by the build system */
seq_t a(idx_t n)
{
    /* Handle requests below the offset (undefined behaviour allowed) */
    if (n < 4) {
        return (seq_t)0;
    }

    idx_t idx = n - 4;          /* zero‑based index of the term            */
    static seq_t *cache = NULL; /* cache a(4), a(5), ...                 */
    static idx_t cached_len = 0;

    /* Initial terms */
    if (cache == NULL) {
        cached_len = 3;                               /* a(4), a(5), a(6) */
        cache = (seq_t*)malloc(cached_len * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)13;        /* a(4) */
        cache[1] = (seq_t)22;        /* a(5) */
        cache[2] = (seq_t)39;        /* a(6) */
    }

    /* Grow the cache if necessary */
    if (idx >= cached_len) {
        idx_t new_len = idx + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failed */
        cache = new_cache;

        /* Compute missing terms using the recurrence */
        for (idx_t i = cached_len; i <= idx; ++i) {
            seq_t temp = cache[i-1];          /* a(i-1) */
            temp = temp + temp;               /* 2*a(i-1) */
            temp = temp - cache[i-2];         /* 2*a(i-1) - a(i-2) */
            temp = temp + cache[i-3];         /* 2*a(i-1) - a(i-2) + a(i-3) */
            cache[i] = temp;                  /* a(i) */
        }
        cached_len = new_len;
    }

    return cache[idx];
}
