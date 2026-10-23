/*
 * A187500  Let i be in {1,2,3,4} and let r >= 0 be an integer. Let p = {p_1,
 *          p_2, p_3, p_4} = {-2,0,1,2}, n=3*r+p_i, and define a(-2)=0. Then
 *          a(n)=a(3*r+p_i) gives the quantity of H_(9,2,0) tiles in a
 *          subdivided H_(9,i,r) tile after linear scaling by the factor Q^r,
 *          where Q=sqrt(x^2-1) with x=2*cos(Pi/9).
 * https://oeis.org/A187500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-3) - 3*a(n-9) for n ≥ 12, with initial values
 *            a(0)–a(11) = {1,0,0,1,0,1,2,1,2,4,3,5}
 * Range:     u8 n <= 23, u16 n <= 41, u32 n <= 77, u64 n <= 149,
 *            u128 n <= 292, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static size_t a_cache_max = 0; /* highest index computed */

seq_t a(idx_t n)
{
    /* Local copy of the 12 initial values */
    static const seq_t init[12] = {1,0,0,1,0,1,2,1,2,4,3,5};

    /* Initialise cache on first call */
    if (a_cache == NULL) {
        a_cache_cap = 12;
        a_cache = (seq_t*)malloc(a_cache_cap * sizeof(seq_t));
        if (!a_cache) exit(1);
        for (size_t i = 0; i < a_cache_cap; ++i)
            a_cache[i] = init[i];
        a_cache_max = 11;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > a_cache_cap) {
        size_t new_cap = a_cache_cap > 0 ? a_cache_cap : 12;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    if ((size_t)n > a_cache_max) {
        for (size_t k = a_cache_max + 1; k <= (size_t)n; ++k) {
            if (k < 12)
                a_cache[k] = init[k];
            else
                a_cache[k] = 3 * a_cache[k-3] - 3 * a_cache[k-9];
        }
        a_cache_max = (size_t)n;
    }

    return a_cache[n];
}
