/*
 * A116847  Number of permutations of length n which avoid the patterns 123,
 *          51432.
 * https://oeis.org/A116847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1) - 19*a(n-2) + 26*a(n-3) - 19*a(n-4)
                     + 7*a(n-5) - a(n-6)   (n >= 7)
   Initial terms: a(1)=1, a(2)=2, a(3)=5, a(4)=14, a(5)=41, a(6)=119 */

#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;   /* cache[0] unused, cache[i] = a(i) for i >= 1 */
static idx_t cache_len = 0;   /* highest index stored, 0 if uninitialised */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 */

    /* initialise cache with the first 6 terms if not yet done */
    if (cache == NULL) {
        cache = seq_alloc_array(7);   /* indices 0 .. 6 */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 5;
        cache[4] = 14;
        cache[5] = 41;
        cache[6] = 119;
        cache_len = 6;
    }

    /* return cached value if available */
    if (n <= cache_len) return cache[n];

    /* compute terms up to n using the recurrence */
    idx_t new_len = n;
    seq_t *new_cache = seq_alloc_array(new_len + 1);  /* indices 0 .. n */

    /* copy existing cached values */
    for (idx_t i = 1; i <= cache_len; ++i)
        new_cache[i] = cache[i];
    new_cache[0] = 0;

    for (idx_t i = cache_len + 1; i <= new_len; ++i) {
        /* use new_cache for all previous terms */
        seq_t pos = new_cache[i-1] * 7
                  + new_cache[i-3] * 26
                  + new_cache[i-5] * 7;
        seq_t neg = new_cache[i-2] * 19
                  + new_cache[i-4] * 19
                  + new_cache[i-6];
        new_cache[i] = pos - neg;   /* pos >= neg for all n >= 7 */
    }

    /* replace old cache */
    free_seq_array(cache);
    cache = new_cache;
    cache_len = new_len;

    return cache[n];
}
