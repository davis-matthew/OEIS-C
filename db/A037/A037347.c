/*
 * A037347  Numbers whose base-3 and base-6 expansions have no digits in
 *          common.
 * https://oeis.org/A037347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array, no_common_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A037347 – numbers whose base‑3 and base‑6 expansions have no
   common digits. The function generates the sequence on demand and caches
   the results to support calls in any order. */
/* It follows the OEIS definition: a(n) is the nth integer such that its
   base‑3 digits and base‑6 digits have no overlap. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"   /* provides seq_t *alloc_seq_array(size_t) */
#include "../utils/free_seq_array.h"    /* provides void free_seq_array(seq_t *) */
#include "../utils/no_common_digits.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset = 1 (a(1) is the first term) */

    /* Static state: cache of previously computed sequence values. */
    static seq_t *cache     = NULL;    /* array of seq_t */
    static size_t cache_len = 0;       /* number of elements in cache */
    static size_t cache_cap = 0;       /* allocated capacity */
    static seq_t last_candidate = 0;   /* last integer examined */

    /* Grow the cache if the requested index is beyond what we have. */
    if ((size_t)n > cache_len) {
        /* Determine new capacity (next power of two >= n). */
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;

        /* Allocate a new buffer. */
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (!new_cache) {              /* allocation failure – return 0 */
            return 0;
        }

        /* Copy existing elements to the new buffer. */
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }

        /* Free the old buffer if it existed. */
        if (cache) free_seq_array(cache);

        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Generate additional terms until cache_len == n. */
    while (cache_len < (size_t)n) {
        /* Advance candidate and test the property. */
        do { ++last_candidate; } while (!no_common_digits(last_candidate));
        cache[cache_len++] = last_candidate;
    }

    return cache[n - 1];
}
