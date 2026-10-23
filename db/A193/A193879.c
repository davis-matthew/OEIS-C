/*
 * A193879  Different leap years in the Gregorian and the revised Julian
 *          calendars.
 * https://oeis.org/A193879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   ( N // 9 = 2 | N // 9 = 6 ) XOR ( N // 4 = 0 )
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 0;   /* last integer examined */

    /* a(1) is defined; silently return 0 for n==0 (not used) */
    if (n == 0) return 0;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* grow the cache if needed */
        if (cache_len == cache_cap)
        {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* memory exhausted */
            cache = tmp;
            cache_cap = new_cap;
        }

        idx_t N = last_checked;
        /* (N % 4 == 0) gives 1 if true, else 0 */
        idx_t greg = (N % 4 == 0) ? 1 : 0;
        /* (N % 9 == 2 || N % 9 == 6) gives 1 if true, else 0 */
        idx_t rev  = ((N % 9 == 2) || (N % 9 == 6)) ? 1 : 0;

        if (greg ^ rev)            /* XOR of the two conditions */
        {
            cache[cache_len++] = (seq_t)N;
        }
        last_checked++;            /* advance to next integer */
    }

    return cache[n - 1];
}
