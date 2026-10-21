/*
 * A220739  Number of ways to reciprocally link elements of an 2 X n array
 *          either to themselves or to exactly two horizontal, diagonal and
 *          antidiagonal neighbors, without consecutive collinear links.
 * https://oeis.org/A220739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)-2*a(n-4)-2*a(n-5)+a(n-6) with a(1)=1,
 *            a(2)=2, a(3)=5, a(4)=13, a(5)=32, a(6)=85
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] unused, cache[1..] stores values */
    static idx_t cache_max = 0;   /* highest index currently stored */

    if (n == 0) return (seq_t)0; /* undefined for n=0, guard */

    if (cache == NULL || cache_max < n)
    {
        idx_t need = n + 1; /* +1 to ignore index 0 */
        seq_t *tmp;

        if (cache == NULL)
        {
            tmp = (seq_t*)malloc(need * sizeof(seq_t));
        }
        else
        {
            tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        }

        if (!tmp) abort();      /* allocation failed */
        cache = tmp;

        for (idx_t i = cache_max + 1; i <= n; ++i)
        {
            switch (i)
            {
                case 1: cache[i] = (seq_t)1;      break;
                case 2: cache[i] = (seq_t)2;      break;
                case 3: cache[i] = (seq_t)5;      break;
                case 4: cache[i] = (seq_t)13;     break;
                case 5: cache[i] = (seq_t)32;     break;
                case 6: cache[i] = (seq_t)85;     break;
                default:
                {
                    seq_t v = (seq_t)2 * cache[i-1]   // 2*a(n-1)
                           + (seq_t)2 * cache[i-2]   // 2*a(n-2)
                           - (seq_t)2 * cache[i-4]   // -2*a(n-4)
                           - (seq_t)2 * cache[i-5]   // -2*a(n-5)
                           + cache[i-6];            // +a(n-6)
                    cache[i] = v;
                    break;
                }
            }
        }
        cache_max = n;
    }

    return cache[n];
}
