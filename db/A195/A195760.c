/*
 * A195760  G.f.: A(x) = exp( Sum_{n>=1} 5*5^A112765(n) * x^n/n ), where
 *          A112765 is the exponent of the highest power of 5 dividing n.
 * https://oeis.org/A195760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = A(x^5)*(1-x^5)/(1-x)^5 => a(0)=1, for n≥1 a(n) =
 *            Σ_{m=0}^{⌊n/5⌋} a(m)·C(n-5m+4,4) – Σ_{m=0}^{⌊(n-5)/5⌋}
 *            a(m)·C(n-5-5m+4,4) (C(t,4) = 0 for t<4)
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 112, u64 n <= 937,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* allocate an array that can be grown with realloc, compatible with C and C++ */
#include <stdlib.h>

static seq_t binom4(idx_t p)
/* returns C(p+4,4) = (p+4)(p+3)(p+2)(p+1)/24 for p≥0 */
{
    seq_t res = 1;
    res *= (seq_t)(p + 4);
    res /= 1;
    res *= (seq_t)(p + 3);
    res /= 2;
    res *= (seq_t)(p + 2);
    res /= 3;
    res *= (seq_t)(p + 1);
    res /= 4;
    return res;
}

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;   /* next index to compute */

    /* grow cache if needed */
    if (len <= n)
    {
        size_t new_cap = (cap == 0) ? (n + 1) : cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;

        /* initialise first term */
        if (len == 0)
        {
            cache[0] = 1;
            len = 1;
        }

        /* compute missing values */
        for (size_t i = len; i <= (size_t)n; ++i)
        {
            idx_t idx = (idx_t)i;   /* use idx_t for arithmetic */
            seq_t sum = 0;

            /* first sum */
            idx_t max_m = idx / 5;
            for (idx_t m = 0; m <= max_m; ++m)
            {
                idx_t p = idx - 5 * m;
                sum += cache[m] * binom4(p);
            }

            /* second sum (only if idx >= 5) */
            if (idx >= 5)
            {
                idx_t max_m2 = (idx - 5) / 5;
                for (idx_t m = 0; m <= max_m2; ++m)
                {
                    idx_t p = idx - 5 - 5 * m;
                    sum -= cache[m] * binom4(p);
                }
            }

            cache[i] = sum;
        }

        len = (size_t)n + 1;
    }

    return cache[n];
}
