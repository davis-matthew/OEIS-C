/*
 * A164742  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n - Σ_{k=1..min(8,n)} c_k * a(n-k) * Where p =
 *            {1,2,2,2,2,2,2,2,1} and * c = { -, -5,-5,-5,-5,-5,-5,-5,15 }. *
 *            Derived from the rational generating function * G(t) = (t^8 +
 *            2*t^7 + 2*t^6 + 2*t^5 + 2*t^4 + 2*t^3 + 2*t^2 + 2*t + 1) / *
 *            (15*t^8 - 5*t^7 - 5*t^6 - 5*t^5 - 5*t^4 - 5*t^3 - 5*t^...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t p_coeff[9] = {1,2,2,2,2,2,2,2,1};
static const int    c[9]      = {0,-5,-5,-5,-5,-5,-5,-5,15};

seq_t a(idx_t n)
{
    /* Cache for previously computed values. */
    static seq_t *cache   = NULL;
    static size_t cache_sz = 0;

    size_t idx = (size_t)n;

    if (idx >= cache_sz)
    {
        size_t new_sz = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – unlikely */
        cache = tmp;

        for (size_t i = cache_sz; i <= idx; ++i)
        {
            seq_t val = 0;
            if (i <= 8) val = p_coeff[i];  /* p_i for i <= 8, 0 otherwise */

            for (int k = 1; k <= 8 && i >= (size_t)k; ++k)
            {
                if (c[k] < 0)              /* add |c_k| * a[i-k] */
                    val += (seq_t)(-c[k]) * cache[i - k];
                else                       /* subtract c_k * a[i-k] (c_k == 15) */
                    val -= (seq_t)c[k] * cache[i - k];
            }
            cache[i] = val;
        }
        cache_sz = new_sz;
    }

    return cache[idx];
}
