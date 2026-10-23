/*
 * A025755  10th-order Patalan numbers (generalization of Catalan numbers).
 * https://oeis.org/A025755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*(10*n-11)/n * a(n-1), a(0)=1, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   gcd_seq_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* NULL, size_t */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdbool.h>
#include "../utils/gcd_seq_idx.h"

/* Euclidean gcd for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Cache for memoisation of the sequence. */
static seq_t  *cache     = NULL;   /* sequence values */
static idx_t   cache_cap = 0;      /* allocated capacity */
static idx_t   cache_len = 0;      /* number of values currently stored (next index to compute) */

/* Ensure the cache can hold at least (n+1) elements. */
static void ensure_cache(idx_t n)
{
    idx_t needed = n + 1;
    if (cache_cap < needed) {
        idx_t new_cap = needed * 2;
        if (new_cap < needed) new_cap = needed;   /* avoid overflow */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (new_cache == NULL) {
            /* In real code we would handle allocation failure. */
            abort();
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0 || n == 1) return 1;

    /* Allocate or grow the cache to accommodate n. */
    ensure_cache(n);

    /* If we already have a computed value, return it. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Initialise cache on the first use. */
    if (cache_len == 0) {
        cache[0] = 1;
        cache[1] = 1;
        cache_len = 2;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1) * 10 * (10*i-11) / i  */
        idx_t num = 10 * (10 * i - 11);   /* numerator (integer) */
        idx_t denom = i;                 /* denominator (integer) */

        /* Reduce common factors between numerator and denominator. */
        idx_t g = gcd_idx(num, denom);
        if (g > 1) {
            num /= g;
            denom /= g;
        }

        seq_t prev = cache[i - 1];

        /* Reduce denominator with prev. */
        idx_t g2 = gcd_seq_idx(prev, denom);
        seq_t prev_red = prev / g2;
        idx_t denom2 = denom / g2;

        /* Reduce numerator with the remaining denominator. */
        idx_t g3 = gcd_seq_idx(num, denom2);
        seq_t num_red = (seq_t)num / g3;
        denom2 /= g3;   /* should now be 1 for exactness */

        seq_t next;
        if (denom2 == 1) {
            next = prev_red * num_red;
        } else {
            /* As a safety fallback: divide after multiplication. */
            next = (prev_red * num_red) / (seq_t)denom2;
        }

        cache[i] = next;
    }

    /* All terms up to n are now computed. */
    cache_len = n + 1;
    return cache[n];
}
