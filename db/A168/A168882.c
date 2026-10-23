/*
 * A168882  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n + 11 * Σ_{i=1}^{min(n,20)} a(n-i) - (n≥21 ? 66 *
 *            a(n-21) : 0) with p_0 = 1, p_1 … p_20 = 2, p_21 = 1, p_n = 0 for
 *            n > 21
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence follows from the rational generating function
   given in the OEIS entry for A168882. */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static size_t cache_capacity = 0;

static void grow_cache(idx_t need)
{
    if (need <= cache_capacity)
        return;
    size_t newcap = cache_capacity > 0 ? cache_capacity * 2 : 64;
    while (newcap < (size_t)need)
        newcap *= 2;
    seq_t *newptr = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    if (!newptr) abort();          /* allocation failure */
    cache = newptr;
    cache_capacity = newcap;
}

seq_t a(idx_t n)
{
    if (n >= cache_size) {
        grow_cache(n + 1);
        /* compute missing values */
        for (idx_t cur = cache_size; cur <= n; ++cur) {
            seq_t p = 0;
            if (cur == 0)          p = 1;
            else if (cur <= 20)    p = 2;
            else if (cur == 21)    p = 1;   /* only needed for completeness */
            else                  p = 0;

            if (cur == 0) {
                cache[0] = 1;
                continue;
            }

            seq_t sum20 = 0;
            for (idx_t i = 1; i <= 20 && cur >= i; ++i)
                sum20 += cache[cur - i];

            seq_t val = p + sum20 * (seq_t)11;
            if (cur >= 21)
                val -= cache[cur - 21] * (seq_t)66;

            cache[cur] = val;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
