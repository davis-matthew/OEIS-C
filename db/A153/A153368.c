/*
 * A153368  Number of zig-zag paths from top to bottom of a rectangle of width
 *          11 with n rows.
 * https://oeis.org/A153368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of zig‑zag words of length n over the 11‑letter
 *            alphabet * (adjacent letters differ by ±1). * Recurrence (used
 *            for verification only): * a(1)=11, a(2)=20, a(3)=38, a(4)=72,
 *            a(5)=138, a(6)=264, * a(n)=6*a(n-2)-9*a(n-4)+2*a(n-6) for n≥7. *
 *            The implementation computes a(n) by dynamic p...
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, but guard against 0 */

    /* static data shared among calls */
    static idx_t cached_n = 0;      /* largest n for which we have a(n) cached */
    static seq_t *cache  = NULL;    /* array cache[1..cached_n] */
    static seq_t cur[11];           /* current walk counts for step k */
    static seq_t nxt[11];           /* temporary array for step k+1 */

    /* initialise on first call */
    if (!cached_n)
    {
        cache = (seq_t *)malloc(sizeof(seq_t) * 2); /* indices 0 and 1 */
        cache[1] = 11;
        for (int i = 0; i < 11; ++i) cur[i] = 1;
        cached_n = 1;
    }

    /* return cached value if already computed */
    if (n <= cached_n) return cache[n];

    /* extend cache up to n */
    idx_t new_n = n;
    if (new_n > cached_n)
    {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_n + 1));
        if (!tmp) return 0;           /* allocation failed: return 0 */
        cache = tmp;

        for (idx_t k = cached_n + 1; k <= new_n; ++k)
        {
            /* compute walk counts for step k from step k-1 */
            for (int i = 0; i < 11; ++i)
            {
                seq_t v = 0;
                if (i > 0)   v += cur[i - 1];
                if (i < 10)  v += cur[i + 1];
                nxt[i] = v;
            }

            /* sum over all end positions to obtain a(k) */
            seq_t total = 0;
            for (int i = 0; i < 11; ++i) total += nxt[i];
            cache[k] = total;

            /* prepare for next iteration */
            for (int i = 0; i < 11; ++i) cur[i] = nxt[i];
        }
        cached_n = new_n;
    }

    return cache[n];
}
