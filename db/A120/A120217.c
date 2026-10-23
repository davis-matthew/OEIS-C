/*
 * A120217  Start with 100057 and repeatedly reverse the digits and add 2 to
 *          get the next term.
 * https://oeis.org/A120217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 945, u64 n <= 945, u128 n <= 945,
 *            big any n
 * Helpers:   reverse_decimal, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 100057,
   a(n) = reverse_decimal(a(n-1)) + 2  for n > 1 */
#include <stddef.h>
#include "../utils/reverse_decimal.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset < 1 is undefined */

    /* Static cache for the sequence, 0‑based indexing (cache[0] = a(1)) */
    static seq_t *cache      = NULL;   /* pointer to allocated array   */
    static size_t cache_len  = 0;      /* number of terms stored        */
    static size_t cache_cap  = 0;      /* array capacity in terms       */

    /* Grow the cache if necessary (odds of doubling to keep O(n) overall) */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise with the first term on first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)100057;
        cache_len = 1;
    }

    /* Compute any missing terms up to the requested index */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        cache[i] = reverse_decimal(cache[i - 1]) + (seq_t)2;
    }
    cache_len = (size_t)n;

    return cache[n - 1];
}
