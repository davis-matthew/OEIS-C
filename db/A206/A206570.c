/*
 * A206570  Number of n X 1 0..3 arrays avoiding the pattern z-1 z-1 z in any
 *          row, column or nw-to-se diagonal.
 * https://oeis.org/A206570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=16, a(3)=61, a(4)=232, a(5)=882, a(6)=3353,
 *            a(7)=12747 For n>7: a(n) = 4*a(n-1) - 3*a(n-3) + 2*a(n-5) -
 *            a(n-7)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes for memory allocation */
#include <stdlib.h>

/* Compute A206570 sequence element. */
seq_t a(idx_t n)
{
    if (n < 1)   /* undefined for n < 1; return 0 */
        return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Grow cache if needed */
    if (n >= cache_size)
    {
        idx_t new_size = n + 1;          /* we use 1‑based indices */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)                /* allocation failure – abort */
            return (seq_t)0;
        cache = tmp;

        /* Initialise new entries */
        for (idx_t i = cache_size; i < new_size; ++i)
        {
            switch (i)
            {
                case 1:  cache[i] = (seq_t)4;      break;
                case 2:  cache[i] = (seq_t)16;     break;
                case 3:  cache[i] = (seq_t)61;     break;
                case 4:  cache[i] = (seq_t)232;    break;
                case 5:  cache[i] = (seq_t)882;    break;
                case 6:  cache[i] = (seq_t)3353;   break;
                case 7:  cache[i] = (seq_t)12747;  break;
                default:
                    /* recurrence: a(n) = 4*a(n-1) - 3*a(n-3) + 2*a(n-5) - a(n-7) */
                    seq_t v = (seq_t)4 * cache[i - 1];
                    v -= (seq_t)3 * cache[i - 3];
                    v += (seq_t)2 * cache[i - 5];
                    v -= cache[i - 7];
                    cache[i] = v;
                    break;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
