/*
 * A210348  1/4 of the number of (n+1) X 2 0..3 arrays with every 2 X 2
 *          subblock having one, two or three distinct clockwise edge
 *          differences.
 * https://oeis.org/A210348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Empirical recurrence for OEIS A210348:
 *     a(1) = 40,  a(2) = 431,  a(3) = 4615,  a(4) = 49484,  a(5) = 530580
 *     a(n) = 14*a(n-1) - 30*a(n-2) - 66*a(n-3) + 104*a(n-4) + 136*a(n-5)  for n ≥ 6
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* below offset – behaviour irrelevant */

    /* Grow cache to hold index n (cache indices start at 0, unused) */
    if ((size_t)n + 1 > cache_len)
    {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failure – return 0 */
        cache = tmp;

        /* Initialize first five terms if they are not already set */
        for (size_t i = cache_len; i <= (size_t)n; ++i)
        {
            if (i == 1) cache[i] = (seq_t)40;
            else if (i == 2) cache[i] = (seq_t)431;
            else if (i == 3) cache[i] = (seq_t)4615;
            else if (i == 4) cache[i] = (seq_t)49484;
            else if (i == 5) cache[i] = (seq_t)530580;
            else
            {
                seq_t term = (seq_t)14 * cache[i-1]
                           - (seq_t)30 * cache[i-2]
                           - (seq_t)66 * cache[i-3]
                           + (seq_t)104 * cache[i-4]
                           + (seq_t)136 * cache[i-5];
                cache[i] = term;
            }
        }

        cache_len = new_len;
    }

    return cache[n];
}
