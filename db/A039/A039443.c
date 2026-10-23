/*
 * A039443  Numbers whose base-11 representation has the same nonzero number
 *          of 0's and 6's.
 * https://oeis.org/A039443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   eq_zero_six
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039443
   a(n) = n-th positive integer whose base‑11 representation has the same
   nonzero number of digit 0’s and digit 6’s.
*/
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/eq_zero_six.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1 */

    /* Static cache to store previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next = (seq_t)1;  /* smallest positive integer to test */

    /* Initialise cache on first call. */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;    /* allocation failure */
    }

    /* Grow cache until we have at least n terms. */
    while (cache_len < n) {
        if (eq_zero_six(next)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next;
        }
        next++;
    }

    return cache[n - 1];
}
