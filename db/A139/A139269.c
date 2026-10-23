/*
 * A139269  Twice lucky numbers.
 * https://oeis.org/A139269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * A000959(n)
 * Range:     u8 n <= 27, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   compute_lucky_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_lucky_upto.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset is 1 */

    /* Cached lucky numbers (not doubled). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (cache_len >= (size_t)n) {
        return cache[(size_t)n - 1] * (seq_t)2;
    }

    /* Estimate a limit that should contain at least n lucky numbers.
       For n up to 10,000 the n*10 bound is more than sufficient. */
    seq_t limit = (seq_t)n * 10;
    if (limit < 10) limit = 10;          /* small safety guard */

    seq_t *new_cache = NULL;
    size_t new_len = 0;

    /* Try progressively larger limits until we have enough terms. */
    while (1) {
        compute_lucky_upto(limit, &new_cache, &new_len);
        if (!new_cache) return 0;       /* allocation failure */

        if (new_len >= (size_t)n) {
            break;                      /* enough terms gathered */
        }

        /* Not enough: free what we just computed and double the limit. */
        free(new_cache);
        new_cache = NULL;
        new_len = 0;
        limit *= 2;
    }

    /* Replace the old cache (if any). */
    free(cache);
    cache = new_cache;
    cache_len = new_len;

    return cache[(size_t)n - 1] * (seq_t)2;
}
