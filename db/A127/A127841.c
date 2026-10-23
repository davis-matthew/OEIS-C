/*
 * A127841  a(1)=1, a(2)=...=a(7)=0, a(n) = a(n-7)+a(n-6) for n>7.
 * https://oeis.org/A127841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-7) + a(n-6), with a(1)=1, a(2)=...=a(7)=0 (index
 *            starts at 1)
 * Range:     u8 n <= 77, u16 n <= 129, u32 n <= 233, u64 n <= 440,
 *            u128 n <= 855, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow calls in arbitrary order */
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of populated entries; cache[n-1] holds a(n) */

    if (n == 0)          /* undefined, but return 0 to avoid UB */
        return (seq_t)0;

    /* Expand cache if necessary */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)           /* allocation failure */
            return (seq_t)0;
        cache = tmp;

        /* Compute newly required terms */
        for (size_t i = cache_len; i < new_len; ++i) {
            idx_t idx = (idx_t)i + 1; /* map array index to sequence index */

            if (idx == 1)
                cache[i] = (seq_t)1;
            else if (idx >= 2 && idx <= 7)
                cache[i] = (seq_t)0;
            else {
                /* recurrence: a(idx) = a(idx-7) + a(idx-6) */
                size_t off7 = (size_t)(idx - 7) - 1; /* idx-7  => array index */
                size_t off6 = (size_t)(idx - 6) - 1; /* idx-6  => array index */
                cache[i] = cache[off7] + cache[off6];
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n - 1];
}
