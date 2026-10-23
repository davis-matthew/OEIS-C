/*
 * A187499  Let i be in {1,2,3,4} and let r >= 0 be an integer. Let p = {p_1,
 *          p_2, p_3, p_4} = {-2,0,1,2}, n=3*r+p_i, and define a(-2)=1. Then
 *          a(n)=a(3*r+p_i) gives the quantity of H_(9,1,0) tiles in a
 *          subdivided H_(9,i,r) tile after linear scaling by the factor Q^r,
 *          where Q=sqrt(x^2-1) with x=2*cos(Pi/9).
 * https://oeis.org/A187499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-3) - 3*a(n-9) for n ≥ 12 * Initial values: a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=1, * a(5)=0, a(6)=0, a(7)=1,
 *            a(8)=1, a(9)=1, * a(10)=3, a(11)=2
 * Range:     u8 n <= 26, u16 n <= 43, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[12] = {
    0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 3, 2
};

static seq_t *cache = NULL;      /* memoized values */
static idx_t  cache_size = 0;    /* number of already stored values */

seq_t a(idx_t n)
{
    if (n < 12)
        return init[n];

    /* Ensure cache is allocated and all initial values are stored */
    if (cache == NULL) {
        cache_size = 12;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* allocation failure, small fallback */
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init[i];
    }

    if (n >= cache_size) {
        idx_t old_size = cache_size;
        cache_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (tmp == NULL)
            return 0; /* reallocation failure, small fallback */
        cache = tmp;

        /* Fill any newly needed values */
        for (idx_t i = old_size; i <= n; ++i) {
            if (i < 12) {
                cache[i] = init[i];
            } else {
                /* a(i) = 3*a(i-3) - 3*a(i-9) */
                seq_t t1 = (seq_t)(3 * cache[i - 3]);
                seq_t t2 = (seq_t)(3 * cache[i - 9]);
                cache[i] = t1 - t2;
            }
        }
    }

    return cache[n];
}
