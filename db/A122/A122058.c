/*
 * A122058  Expansion of x*(1 + 4*x + 6*x^2 + 6*x^3)/((1-x)*(1 - 11*x^2 -
 *          12*x^3)).
 * https://oeis.org/A122058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1,a(2)=5,a(3)=22,a(4)=84;
   for n>=5, a(n)=a(n-1)+11*a(n-2)+a(n-3)-12*a(n-4) */
#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[i] holds a(i+1) */
static size_t cache_len = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* current capacity of cache */

static inline void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n==0, return 0 */

    size_t idx = (size_t)n;
    if (idx == 0)
        return 0;

    /* extend cache up to index n */
    if (cache_len < idx)
    {
        ensure_cache(idx);
        for (size_t j = cache_len + 1; j <= idx; ++j)
        {
            seq_t prev1 = 0, prev2 = 0, prev3 = 0, prev4 = 0;
            if (j >= 2)      /* a(j-1) */
                prev1 = cache[j - 2];
            if (j >= 3)      /* a(j-2) */
                prev2 = cache[j - 3];
            if (j >= 4)      /* a(j-3) */
                prev3 = cache[j - 4];
            if (j >= 5)      /* a(j-4) */
                prev4 = cache[j - 5];

            /* recurrence: a(j) = a(j-1)+11*a(j-2)+a(j-3)-12*a(j-4) + p_j   */
            seq_t sum = prev1 + (seq_t)11 * prev2 + prev3 - (seq_t)12 * prev4;

            /* add the polynomial term p_j for j = 1..4 */
            switch (j)
            {
                case 1: sum += 1; break;
                case 2: sum += 4; break;
                case 3: sum += 6; break;
                case 4: sum += 6; break;
                default: break;   /* p_j = 0 for j > 4 */
            }

            cache[j - 1] = sum;
        }
        cache_len = idx;
    }

    return cache[idx - 1];
}
