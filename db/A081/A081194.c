/*
 * A081194  a(n) = 8*a(n-1) -15*a(n-2), a(0)=1, a(1)=16.
 * https://oeis.org/A081194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 15*a(n-2) ; a(0) = 1, a(1) = 16 Computes the
 *            OEIS sequence A081194 using memoised recurrence
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Handle the initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;

    /* Initialise the cache on first use. */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* unlikely: out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)16;
    }

    /* Grow and fill the cache if the requested index is beyond what we have. */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – compute the requested term on the fly. */
            seq_t prev2 = cache[0];
            seq_t prev1 = cache[1];
            seq_t cur;
            for (idx_t i = 2; i <= n; ++i) {
                cur = prev1 * ((seq_t)8) - prev2 * ((seq_t)15);
                prev2 = prev1;
                prev1 = cur;
            }
            return cur;
        }
        cache = new_cache;
        seq_t prev2 = cache[cache_len - 2];
        seq_t prev1 = cache[cache_len - 1];
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t cur = prev1 * ((seq_t)8) - prev2 * ((seq_t)15);
            cache[i] = cur;
            prev2 = prev1;
            prev1 = cur;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
