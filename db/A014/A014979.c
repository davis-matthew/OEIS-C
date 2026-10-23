/*
 * A014979  Numbers that are both triangular and pentagonal.
 * https://oeis.org/A014979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A014979: Numbers that are both triangular and pentagonal
   Recurrence: a(1) = 0, a(2) = 1,
                a(n) = 194 * a(n-1) - a(n-2) + 16 for n >= 3 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */
    if (n == 1) return 0;
    if (n == 2) return 1;

    /* Static cache: cache[i-1] stores a(i) for i >= 1 */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */

    /* Ensure cache can hold up to n */
    if (cache_len < n) {
        idx_t new_len = n;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* Initialize first terms if needed */
        if (new_len >= 1) cache[0] = 0;
        if (new_len >= 2) cache[1] = 1;

        /* Compute missing terms using the recurrence */
        idx_t start = 3;
        if (cache_len > 2) start = cache_len + 1;   /* start from next needed */
        for (idx_t i = start; i <= new_len; ++i) {
            cache[i-1] = ((seq_t)194) * cache[i-2] - cache[i-3] + ((seq_t)16);
        }
        cache_len = new_len;
    }

    return cache[n-1];
}
