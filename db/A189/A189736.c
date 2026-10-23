/*
 * A189736  a(1)=3, a(2)=2, a(n)=3*a(n-1) + 2*a(n-2).
 * https://oeis.org/A189736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 70,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=3, a(2)=2, a(n)=3*a(n-1)+2*a(n-2) for n≥3 */
#include <stddef.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    static seq_t *cache = NULL;   /* cached terms */
    static size_t  cap   = 0;     /* allocated array capacity */
    static idx_t   len   = 0;     /* number of computed terms */

    /* initialise cache with the first two terms */
    if (cap == 0) {
        cache = seq_alloc_new_array(2);
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)2;
        cap   = 2;
        len   = 2;
    }

    /* grow storage if needed */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap * 2 : 2;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        memcpy(new_cache, cache, len * sizeof(seq_t));
        free_seq_array(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute missing terms */
    if (n > len) {
        for (idx_t i = len; i < n; ++i) {
            /* a(i+1) = 3*a(i) + 2*a(i-1) (indices 0‑based) */
            cache[i] = (seq_t)3 * cache[i - 1] + (seq_t)2 * cache[i - 2];
        }
        len = n;
    }

    return cache[n - 1];
}
