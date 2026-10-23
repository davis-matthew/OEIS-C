/*
 * A166367  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0 a(n)=c_n+7·(a(n-1)+…+a(n-10))−28·a(n-11) where
 *            c_n = 1 if n=0 or 11, 2 if 1≤n≤10, 0 otherwise. This is derived
 *            from the rational generating function
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t coeff(idx_t n)
{
    if (n == 0 || n == 11)
        return (seq_t)1;
    if (n <= 10)
        return (seq_t)2;
    return (seq_t)0;
}

seq_t a(idx_t n)
{
    /* cache of previously computed terms */
    static seq_t *cache        = NULL;   /* dynamic array of a(k) */
    static idx_t   cache_size  = 0;      /* allocated capacity */
    static idx_t   computed    = 0;      /* number of terms stored (next free index) */

    /* initialise cache with a(0) */
    if (cache == NULL) {
        cache_size = 16;                 /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL)
            return (seq_t)0;             /* allocation failed */
        cache[0] = (seq_t)1;
        computed = 1;                   /* next free index is 1 */
    }

    /* extend cache if required */
    if (n >= computed) {
        idx_t want = n + 1;
        if (want > cache_size) {
            idx_t new_cap = cache_size;
            while (new_cap < want)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (tmp == NULL)
                return (seq_t)0;         /* allocation failed */
            cache      = tmp;
            cache_size = new_cap;
        }

        /* compute missing terms up to n */
        for (idx_t i = computed; i <= n; ++i) {
            seq_t sum   = coeff(i);
            seq_t temp  = 0;

            for (idx_t j = 1; j <= 10; ++j) {
                if (i >= j)
                    temp += cache[i - j];
            }
            sum += temp * (seq_t)7;
            if (i >= 11)
                sum -= cache[i - 11] * (seq_t)28;

            cache[i] = sum;
        }
        computed = n + 1;
    }

    return cache[n];
}
