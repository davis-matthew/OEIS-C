/*
 * A063129  Dimension of the space of weight 2n cusp forms for Gamma_0( 61 ).
 * https://oeis.org/A063129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A063129: Dimension of the space of weight 2n cusp forms for Γ₀(61).
   The generating function is
        F(x) = x*(x^5+5x^4+16x^3+21x^2+15x+4) /
               ((x-1)^2*(x+1)*(x^2+x+1))
   Expanding (x-1)^2*(x+1)*(x^2+x+1) gives 1 - x^2 - x^3 + x^5.
   Hence the coefficients a[n] satisfy the linear recurrence
        a[n] = a[n-2] + a[n-3] - a[n-5] + p[n],
   where p[n] is the coefficient of x^n in the numerator:
        p[0] = 0,
        p[1] = 4,   p[2] = 15,   p[3] = 21,
        p[4] = 16,  p[5] = 5,    p[6] = 1,
        p[n] = 0 for n ≥ 7.
   For n ≥ 7 the recurrence becomes homogeneous:
        a[n] = a[n-2] + a[n-3] - a[n-5].      */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0..N] for computed terms   */
static idx_t cache_size = 0;  /* current size of the cache       */

static const seq_t p_coeff[7] = {0, 4, 15, 21, 16, 5, 1};

seq_t a(idx_t n)
{
    /* Make sure the cache contains the term a[n]. */
    if (n >= cache_size)
    {
        idx_t old = cache_size;
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_sz * sizeof(seq_t));
        if (!tmp)
            return 0;  /* allocation failure – return 0 as a safe fallback */

        cache = tmp;
        cache_size = new_sz;

        /* Compute new entries iteratively.                */
        /* cache[0] is defined as 0 (by definition).      */
        for (idx_t i = old; i <= n; ++i)
        {
            seq_t val = 0;

            if (i >= 2)
                val += cache[i - 2];
            if (i >= 3)
                val += cache[i - 3];
            if (i >= 5)
                val -= cache[i - 5];

            if (i <= 6)
                val += p_coeff[(idx_t)i];   /* p[i] is zero for i > 6 */

            cache[i] = val;
        }
    }

    return cache[n];
}
