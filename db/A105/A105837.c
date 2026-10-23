/*
 * A105837  Numbers n such that n^2 = 11*m^2 + 11*m + 1.
 * https://oeis.org/A105837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=131,
   a(3)=398*a(1)+a(2),
   a(4)=398*a(2)+a(1),
   a(n)=398*a(n-2)-a(n-4) for n>4 */

/* Uses a simple dynamic array for memoisation.  The array never
   grows below 2 elements, so the first two terms are always
   available.  realloc cast is required so the code also compiles as
   C++. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;          /* array of computed values */
    static idx_t cache_len = 0;          /* last index that is defined */
    static idx_t cache_cap = 0;          /* allocated capacity (>= cache_len+1) */

    if (n < 1)         /* undefined for n < 1, but never called */
        return (seq_t)0;

    /* Ensure adequate capacity */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap > 0 ? cache_cap << 1 : 8;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first two terms if necessary */
    if (cache_len < 2) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)131;
        cache_len = 2;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = (cache_len < 1 ? 1 : cache_len + 1); i <= n; ++i) {
        if (i == 3) {
            cache[i] = (seq_t)398 * cache[1] + cache[2];
        } else if (i == 4) {
            cache[i] = (seq_t)398 * cache[2] + cache[1];
        } else { /* i >= 5 */
            cache[i] = (seq_t)398 * cache[i - 2] - cache[i - 4];
        }
    }
    if (n > cache_len)
        cache_len = n;

    return cache[n];
}
