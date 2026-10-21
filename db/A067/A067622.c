/*
 * A067622  Consider the power series (x + 1)^(1/3) = 1 + x/3-x^2/9 + 5x^3/81
 *          + ...; sequence gives numerators of coefficients.
 * https://oeis.org/A067622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 12, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067622: Numerators of (1 + x)^(1/3) */
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Cache numerators and denominators up to the current highest index. */
    static seq_t *nums = NULL;
    static seq_t *dens = NULL;
    static size_t sz   = 0;   /* number of stored values; nums[0..sz-1] valid */
    static size_t cap  = 0;   /* allocated capacity in elements */

    /* Resize cache if necessary. */
    if ((size_t)n + 1 > cap)
    {
        size_t newcap = cap > 0 ? cap : 1;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp_nums = (seq_t *)realloc(nums, newcap * sizeof(seq_t));
        seq_t *tmp_dens = (seq_t *)realloc(dens, newcap * sizeof(seq_t));
        nums = tmp_nums;
        dens = tmp_dens;
        cap  = newcap;
    }

    /* Initialise the first term: (1 + x)^(1/3) has a(0) = 1. */
    if (sz == 0)
    {
        nums[0] = (seq_t)1;
        dens[0] = (seq_t)1;
        sz = 1;
    }

    /* Fill in any missing terms up to n. */
    for (idx_t i = sz; i <= n; ++i)
    {
        seq_t prev_num = nums[i-1];
        seq_t prev_den = dens[i-1];

        /* Recurrence factor: ((1/3) - (i-1)) / i = (1 - 3*(i-1)) / (3*i). */
        seq_t mult_num   = (seq_t)(1 - 3 * (seq_t)(i-1));
        seq_t mult_den   = (seq_t)(3 * (seq_t)i);

        seq_t num_new = prev_num * mult_num;
        seq_t den_new = prev_den * mult_den;

        seq_t g = gcd(num_new > (seq_t)0 ? num_new : -num_new, den_new);
        if (g != (seq_t)0)
        {
            num_new /= g;
            den_new /= g;
        }

        nums[i] = num_new;
        dens[i] = den_new;
    }

    sz = (size_t)n + 1;
    return nums[n];
}
