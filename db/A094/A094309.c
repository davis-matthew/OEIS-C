/*
 * A094309  Number of (s(0), s(1), ..., s(n)) such that 0 < s(i) < 6 and |s(i)
 *          - s(i-1)| <= 1 for i = 1,2,...,n, s(0) = 2, s(n) = 5.
 * https://oeis.org/A094309
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3)
   Initial values: a(3)=1, a(4)=4, a(5)=14 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Before the offset, behaviour is irrelevant */
    if (n < 3)
        return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest index stored in cache */

    /* initialise base cases once */
    if (cache_size < 5) {
        idx_t new_size = 6;                     /* indices 0 .. 5 */
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)14;
        cache_size = 5;
    }

    /* extend cache up to requested index */
    if (n > cache_size) {
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t temp = 4 * cache[i - 1];
            temp -= 2 * cache[i - 2];
            temp -= 4 * cache[i - 3];
            cache[i] = temp;
        }
        cache_size = n;
    }

    return cache[n];
}
