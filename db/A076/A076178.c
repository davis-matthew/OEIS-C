/*
 * A076178  a(n) = 2*n^2 - A077071(n).
 * https://oeis.org/A076178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*(popcount(n)-1), a(0) = 0 (popcount(n) =
 *            number of 1‑bits in the binary representation of n)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static seq_t *cache = NULL;      /* cached values a(0..…) */
static idx_t   cache_len = 0;    /* number of cached entries */

/* Return the value of the sequence at index n. */
seq_t a(idx_t n)
{
    /* Ensure cache is at least one entry (for a(0)). */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = 0;
    }

    /* Grow cache if necessary. */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        /* Compute missing terms.  Use 64‑bit popcount of the index integer. */
        for (idx_t i = cache_len; i <= n; ++i) {
            unsigned int pc = __builtin_popcountll((unsigned long long)i);
            /* popcount(i) >= 1 for i>=1, so pc-1 fits in unsigned. */
            seq_t increment = (seq_t)2 * (seq_t)(pc - 1);
            cache[i] = cache[i-1] + increment;
        }
        cache_len = new_len;
    }

    return cache[n];
}
