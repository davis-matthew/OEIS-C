/*
 * A220191  Majority value maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements equal to at least half of
 *          their king-move neighbors in a random 0..2 n X 2 array.
 * https://oeis.org/A220191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(2) = 6, a(3) = 25, a(4) = 88, a(5) = 297, a(6) =
 *            1009, a(7) = 3464, a(8) = 11908, a(9) = 40900, a(10) = 140420,
 *            a(n) = 2*a(n-1) + 2*a(n-2) + 8*a(n-3) + 2*a(n-4) + 16*a(n-5) +
 *            2*a(n-6) + 6*a(n-7) for n>10
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* cache holds already computed values; 0‑based index, but a(0) unused */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of allocated slots (index 0 … cache_len-1) */

    if (n == 0)
        return (seq_t)0;           /* undefined offset, return 0 */

    /* we need to store values up to index n (inclusive) */
    size_t required = (size_t)n + 1;

    if (cache_len < required)
    {
        /* realloc the cache; cast needed because realloc returns void* */
        seq_t *new_cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;       /* allocation failure (unlikely for valid inputs) */

        cache = new_cache;

        /* fill newly allocated entries */
        for (size_t i = cache_len; i < required; ++i)
        {
            if (i == 0)
                continue;       /* a(0) is unused */

            /* initial constants for n = 1 … 10 */
            switch ((int)i)
            {
                case 1:  cache[1]  = (seq_t)2;    break;
                case 2:  cache[2]  = (seq_t)6;    break;
                case 3:  cache[3]  = (seq_t)25;   break;
                case 4:  cache[4]  = (seq_t)88;   break;
                case 5:  cache[5]  = (seq_t)297;  break;
                case 6:  cache[6]  = (seq_t)1009; break;
                case 7:  cache[7]  = (seq_t)3464; break;
                case 8:  cache[8]  = (seq_t)11908;break;
                case 9:  cache[9]  = (seq_t)40900;break;
                case 10: cache[10] = (seq_t)140420;break;
                default:
                {
                    /* recurrence for n > 10 */
                    seq_t res = cache[i-1] << 1;           /* 2*a(n-1) */
                    res += cache[i-2] << 1;                /* 2*a(n-2) */
                    res += cache[i-3] << 3;                /* 8*a(n-3) */
                    res += cache[i-4] << 1;                /* 2*a(n-4) */
                    res += cache[i-5] << 4;                /* 16*a(n-5) */
                    res += cache[i-6] << 1;                /* 2*a(n-6) */
                    res += (cache[i-7] << 1) + (cache[i-7] << 2); /* 6*a(n-7) */
                    cache[i] = res;
                }
                break;
            }
        }
        cache_len = required;
    }

    return cache[n];
}
