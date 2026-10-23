/*
 * A164500  Number of binary strings of length n with no substrings equal to
 *          0010 1001 or 1010.
 * https://oeis.org/A164500
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 85, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164500: number of binary strings of length n
 * avoiding the substrings 00101001 and 1010.
 *
 * Recurrence (from the generating function):
 *   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5)    for n ≥ 9
 * with offset 4 and initial values
 *   a(4)=13, a(5)=21, a(6)=36, a(7)=60, a(8)=100.
 *
 * The function keeps a memoised array of values so subsequent calls
 * that request larger n only compute the new terms.  It works for
 * any seq_t implementation (plain integer types or arbitrary‑precision
 * classes) as long as the standard arithmetic operators are defined
 * for seq_t.
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(n) is defined only for n ≥ 4; below that we return 0. */
    if (n < 4)
        return (seq_t)0;

    /* Static state for memoisation */
    static seq_t *cache = NULL;      /* array of computed a(i) */
    static size_t cache_len = 0;     /* size of cache in elements   */
    static idx_t  max_n = 0;         /* highest n that has been computed */

    /* Initialisation on first call */
    if (cache == NULL)
    {
        /* at least enough space for indices 0 … 8 */
        cache_len = (size_t)((n > 8) ? n + 1 : 9);
        cache     = (seq_t*)calloc(cache_len, sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */

        /* set the initial values for the recurrence */
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)21;
        cache[6] = (seq_t)36;
        cache[7] = (seq_t)60;
        cache[8] = (seq_t)100;
        max_n = 8;
    }
    /* Grow the cache array if needed */
    else if ((size_t)n + 1 > cache_len)
    {
        size_t new_len = cache_len * 2;
        if (new_len < (size_t)n + 1) new_len = (size_t)n + 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */

        /* initialise the newly allocated part to zero */
        for (size_t i = cache_len; i < new_len; ++i)
            tmp[i] = (seq_t)0;

        cache = tmp;
        cache_len = new_len;
    }

    /* Compute any missing values using the recurrence */
    if (n > max_n)
    {
        for (idx_t i = max_n + 1; i <= n; ++i)
        {
            if (i >= 9)
            {
                seq_t v = (seq_t)2 * cache[(size_t)(i-1)]
                          - cache[(size_t)(i-2)]
                          + cache[(size_t)(i-3)]
                          - cache[(size_t)(i-4)]
                          + cache[(size_t)(i-5)];
                cache[(size_t)i] = v;
            }
        }
        max_n = n;
    }

    return cache[(size_t)n];
}
