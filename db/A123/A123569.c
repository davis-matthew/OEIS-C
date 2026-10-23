/*
 * A123569  Number of ways to write n as an ordered sum of 1s, 2s, 3s and 4s
 *          such that no 2 precedes any 1 and no 3 precedes any 1 or 2.
 * https://oeis.org/A123569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ordered sums of 1,2,3,4 with no 2 preceding any
 *            1 and no 3 preceding any 1 or 2. Recurrence (valid for n ≥ 13):
 *            a(n) = a(n-1)+a(n-2)+2*a(n-4)-3*a(n-5)-a(n-6)-a(n-7)-2*a(n-8)+
 *            2*a(n-9)+a(n-10)+a(n-11)+a(n-12) Initial values (n = 0 … 12): 1,
 *            2, 3, 5, 7, 12, 17, 26, 37, 57, 80, 119,...
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <vector>
#else
#include <stdlib.h>
#endif

/* Base values a(0) … a(12) */
static const seq_t base_vals[13] = {
    (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5,  (seq_t)7,
    (seq_t)12,(seq_t)17,(seq_t)26,(seq_t)37,(seq_t)57,
    (seq_t)80,(seq_t)119,(seq_t)168
};

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* C++ implementation using std::vector */
    static std::vector<seq_t> cache;
    if (cache.empty())
    {
        cache.assign(base_vals, base_vals + 13);
    }
    size_t current = cache.size();
    while (current <= (size_t)n)
    {
        seq_t val;
        if (current < 13)
        {
            val = base_vals[current];
        }
        else
        {
            /* recurrence for current >= 13 */
            val =  cache[current-1] + cache[current-2]
                + (seq_t)2 * cache[current-4]
                + (seq_t)2 * cache[current-9]
                + cache[current-10] + cache[current-11]
                + cache[current-12]
                - (seq_t)3 * cache[current-5]
                - cache[current-6] - cache[current-7]
                - (seq_t)2 * cache[current-8];
        }
        cache.push_back(val);
        ++current;
    }
    return cache[(size_t)n];
#else
    /* C implementation using malloc/realloc */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements in cache */

    if (cache_size == 0)
    {
        /* allocate initial size of 13 and copy base values */
        cache = (seq_t *)malloc(13 * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure (unlikely) */
        for (idx_t i = 0; i < 13; ++i)
            cache[i] = base_vals[i];
        cache_size = 13;
    }

    if (n >= cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failure */
        cache = tmp;
        /* compute new elements */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            seq_t val;
            if (i < 13)
            {
                val = base_vals[i];
            }
            else
            {
                /* recurrence for i >= 13 */
                val =  cache[i-1] + cache[i-2]
                    + (seq_t)2 * cache[i-4]
                    + (seq_t)2 * cache[i-9]
                    + cache[i-10] + cache[i-11]
                    + cache[i-12]
                    - (seq_t)3 * cache[i-5]
                    - cache[i-6] - cache[i-7]
                    - (seq_t)2 * cache[i-8];
            }
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
#endif
}
