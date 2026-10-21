/*
 * A080735  a(1)=1, then a(n)=2*a(n-1) if a(n-1) is prime, a(n)=a(n-1)+1
 *          otherwise.
 * https://oeis.org/A080735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = 2*a(n-1) if a(n-1) is prime, else a(n-1)+1 *
 *            (OEIS A080735)
 * Range:     u8 n <= 18, u16 n <= 84, u32 n <= 316, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"   /* provides bool isPrimeGeneric(seq_t) */

static seq_t *cache = NULL;   /* 1‑based index, cache[0] unused */
static idx_t cached_max = 0;  /* largest index that is already stored */

/* Compute a(n) for any n >= 1.  The function is memoized so that
 * repeated calls are O(1) after the initial computation. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety guard for n < 1 */

    /* If already computed, return immediately. */
    if (n <= cached_max) return cache[n];

    /* Resize the cache to hold indices up to n (1‑based). */
    seq_t *new_cache = (seq_t *)realloc(cache,
                                        (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) return 0;             /* allocation failure → zero */
    cache = new_cache;

    /* Initialize the first term if this is the first call. */
    if (cached_max == 0) {
        cache[1] = (seq_t)1;
        cached_max = 1;
        /* If the requested term is 1, we already have it. */
        if (n == 1) return cache[1];
    }

    /* Fill in missing values up to n. */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = isPrimeGeneric(prev) ? (prev * (seq_t)2) : (prev + (seq_t)1);
    }

    cached_max = n;
    return cache[n];
}
