/*
 * A017854  Expansion of 1/(1-x^6-x^7-x^8-x^9-x^10-x^11-x^12-x^13-x^14).
 * https://oeis.org/A017854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 102, u64 n <= 195,
 *            u128 n <= 381, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(1) = a(2) = a(3) = a(4) = a(5) = 0
   a(6) = 1, a(7) = 1, a(8) = 1, a(9) = 1, a(10) = 1, a(11) = 1
   a(12) = 2, a(13) = 3
   a(n) = a(n‑6) + a(n‑7) + a(n‑8) + a(n‑9) + a(n‑10) + a(n‑11) + a(n‑12) + a(n‑13) + a(n‑14)
          for n > 13
*/

#include <stdlib.h>  /* for realloc, abort */

static seq_t *cache = NULL;   /* memoised values */
static size_t cache_size = 0; /* number of elements currently stored */

static void ensure_cache(idx_t n)
{
    if (n < (idx_t)cache_size)        /* already present */
        return;

    /* Grow the cache to hold indices 0..n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();                      /* out of memory */
    cache = tmp;

    /* Compute new entries */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i <= 5) {
            cache[i] = (seq_t)0;
        } else {
            seq_t sum = 0;
            for (idx_t k = 6; k <= 14; ++k) {
                if (i >= k)            /* guard against unsigned underflow */
                    sum += cache[i - k];
            }
            cache[i] = sum;
        }
    }

    cache_size = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
