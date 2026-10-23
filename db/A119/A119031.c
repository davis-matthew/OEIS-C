/*
 * A119031  Add and Reverse: a(n) = the reversal of (a(n-1)+d), case a(1)=1
 *          and d=4.
 * https://oeis.org/A119031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; for n >= 2, a(n) = reverse_decimal(a(n-1) + 4)
 *            (decimal digit reverse)
 * Range:     u8 n <= 8, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a cache of computed terms, extending it as needed.
   It correctly handles seq_t of any width (including arbitrary‑precision builds)
   and uses the provided reverse_decimal helper. */

#include <stdlib.h>
#include "../utils/reverse_decimal.h"

static seq_t *cache = NULL;   /* zero‑based cache, cache[0] == a(1) */
static idx_t  cache_len = 0;  /* number of terms currently stored */

/* Return the nth term of A119031 (1‑based index).  Calls for the same or
   lower n are answered from the cache in O(1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    /* Extend cache if we don't have this many terms yet */
    if (n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)            /* allocation failed – return 0 (unlikely at n<=10000) */
            return (seq_t)0;
        cache = tmp;

        /* Compute terms up to a(n) */
        for (idx_t i = cache_len; i < n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;           /* a(1) */
            } else {
                cache[i] = reverse_decimal(cache[i-1] + (seq_t)4);
            }
        }
        cache_len = n;
    }

    return cache[n-1];   /* cache is zero‑based: a(1) is cache[0] */
}
