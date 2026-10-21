/*
 * A208843  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 20*a(n-2) - 19*a(n-3) Initial terms: a(1)=22,
 *            a(2)=484, a(3)=990. The function is memoised and works for any
 *            order of calls
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>            /* size_t */
#include <stdbool.h>           /* bool */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Static cache for previously computed terms */
static seq_t *a_cache      = NULL;   /* ptr to cache array */
static idx_t  a_cache_fill = 0;      /* highest index computed */
static size_t a_cache_cap  = 0;      /* allocated capacity */

/* Ensure the cache has space for terms up to n, computing any that are missing. */
static void a_ensure_cache(idx_t n)
{
    /* Allocate initial cache if necessary */
    if (a_cache == NULL) {
        a_cache_cap = n + 1;
        if (a_cache_cap < 4U) a_cache_cap = 4U;
        a_cache = seq_alloc(a_cache_cap);   /* seq_alloc is provided by utils */
        /* Set known initial values */
        a_cache[1] = (seq_t)22;
        a_cache[2] = (seq_t)484;
        a_cache[3] = (seq_t)990;
        a_cache_fill = 3;
    }

    /* Grow the cache if needed */
    if (n > a_cache_cap - 1) {
        size_t new_cap = a_cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        a_cache = seq_realloc(a_cache, a_cache_cap, new_cap); /* seq_realloc provided by utils */
        a_cache_cap = new_cap;
    }

    /* Compute any missing terms */
    if (n > a_cache_fill) {
        for (idx_t i = a_cache_fill + 1; i <= n; ++i) {
            if (i <= 3) continue;   /* base values already stored */
            seq_t a1 = a_cache[i - 1];
            seq_t a2 = a_cache[i - 2];
            seq_t a3 = a_cache[i - 3];
            seq_t val = (seq_t)2 * a1;
            val += (seq_t)20 * a2;
            val -= (seq_t)19 * a3;
            a_cache[i] = val;
        }
        a_cache_fill = n;
    }
}

/* Public function returning the n-th term of A208843 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined for n < 1 */
    a_ensure_cache(n);
    return a_cache[n];
}
