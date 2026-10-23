/*
 * A015785  Phi(n) + 6 | sigma(n + 6).
 * https://oeis.org/A015785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   grow_seq, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015785:
   a(n) is the n-th integer k such that phi(k)+6 divides sigma(k+6).
   This implementation scans integers sequentially, using
   the provided phi() and sigma_of() utility functions and caches
   the results for repeated calls. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/phi.h"
#include "../utils/sigma_of.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;
    static idx_t last_k = 0; /* last k that was examined */

    /* grow the cache to hold at least one more element if necessary */
    if (cache_size >= n)
        return cache[n - 1];

    /* extend cache until we have at least n terms */
    while (cache_size < n) {
        /* examine next integer */
        last_k++;

        seq_t phi_val = phi(last_k);
        seq_t a_val = phi_val + (seq_t)6;
        seq_t b_val = sigma_of(last_k + (idx_t)6);

        if (b_val % a_val == 0) {
            /* ensure capacity for a new element */
            if (cache_size == cache_cap)
                grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
