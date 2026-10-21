/*
 * A086841  a(n) = a((a(n-2))*mod(n,2)+a(n-1)*(1-mod(n,2))) + a((n -
 *          a(n-2))*mod(n,2)+(n-a(n-1))*(1-mod(n,2))).
 * https://oeis.org/A086841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   seq_to_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A086841
   Recurrence: a(1) = 1, a(2) = 1
   For n > 2:
       if (n is odd)   a(n) = a(a(n-2)) + a(n - a(n-2))
       if (n is even)  a(n) = a(a(n-1)) + a(n - a(n-1))
   (same as a(n) = a((a(n-2))*mod(n,2)+a(n-1)*(1-mod(n,2))) + a((n - a(n-2))*mod(n,2)+(n-a(n-1))*(1-mod(n,2))) )
*/
#include <stdlib.h>   /* malloc, realloc, abort */
#include "../utils/seq_to_idx.h"

static seq_t *cache = NULL;      /* 1‑indexed array of computed values */
static idx_t   cache_cap = 0;    /* allocated capacity (>= n+2) */
static idx_t   highest      = 0; /* highest index fully computed */

static void ensure_cache(idx_t n)
{
    if (n <= highest) return;          /* already computed */

    /* Allocate space for indices up to n (plus some slack). */
    if (n + 2 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap < n + 2) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the trivial base values if not yet done. */
    if (highest < 2) {
        cache[1] = 1;
        cache[2] = 1;
        highest  = 2;
    }

    /* Compute values from the first uncomputed index up to n. */
    for (idx_t i = highest + 1; i <= n; ++i) {
        if (i & 1U) {                      /* odd n  */
            idx_t idx1 = seq_to_idx(cache[i - 2]);
            seq_t val1 = cache[idx1];
            idx_t idx2 = i - seq_to_idx(cache[i - 2]);
            seq_t val2 = cache[idx2];
            cache[i] = val1 + val2;
        } else {                            /* even n */
            idx_t idx1 = seq_to_idx(cache[i - 1]);
            seq_t val1 = cache[idx1];
            idx_t idx2 = i - seq_to_idx(cache[i - 1]);
            seq_t val2 = cache[idx2];
            cache[i] = val1 + val2;
        }
    }
    highest = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range is undefined */
    ensure_cache(n);
    return cache[n];
}
