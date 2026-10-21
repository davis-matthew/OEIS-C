/*
 * A052127  Sum_{n >= 0} a(n) * x^n / n!^2 = exp(-2*x)/(1-x)^3.
 * https://oeis.org/A052127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * A052124(n) derived from G(x)=exp(-2x)/(1-x)^3
 *            Recurrence (for n ≥ 3): a[n] = 3*n^2*a[n-1] -
 *            3*n^2*(n-1)^2*a[n-2] + n^2*(n-1)^2*(n-2)^2*a[n-3] + (-2)^n * n!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;     /* array of computed a(n) */
static size_t cache_cap = 0;    /* allocated capacity of cache */
static size_t cache_used = 0;   /* number of computed elements in cache */
static seq_t factorial_so_far = 1;   /* (cache_used-1)! for next step */
static seq_t pow2_so_far = 1;        /* 2^(cache_used-1) for next step */

static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Compute all values up to n on demand */
    while (cache_used <= n) {

        idx_t i = cache_used;
        seq_t ni = (seq_t)i;

        /* update factorial and 2^i for this index (except i==0) */
        if (i >= 1) {
            factorial_so_far = factorial_so_far * ni;
            pow2_so_far      = pow2_so_far * (seq_t)2;
        }

        /* ensure space for cache[i] */
        ensure_capacity(cache_used + 1);

        /* base cases */
        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 1;
        } else if (i == 2) {
            cache[2] = 8;
        } else {
            /* term coefficients */
            seq_t tmp1 = ni * ni;                                 /* n^2                     */
            seq_t tmp2 = (ni-1) * (ni-1);                         /* (n-1)^2                 */
            seq_t tmp3 = (ni-2) * (ni-2);                         /* (n-2)^2                 */
            seq_t tmp12 = tmp1 * tmp2;                            /* n^2 * (n-1)^2           */

            /* term1 = 3 * n^2 * a[n-1] */
            seq_t term1 = cache[i-1] * tmp1;
            term1 = term1 * (seq_t)3;

            /* term2 = 3 * n^2*(n-1)^2 * a[n-2] */
            seq_t term2 = cache[i-2] * tmp12;
            term2 = term2 * (seq_t)3;

            /* term3 = n^2*(n-1)^2*(n-2)^2 * a[n-3] */
            seq_t term3 = cache[i-3] * tmp12 * tmp3;

            /* combine terms: t = term1 - term2 + term3 */
            seq_t t = term1 - term2 + term3;

            /* last term: (-2)^n * n! = (sign)*2^n*n! */
            seq_t last = factorial_so_far * pow2_so_far;
            if (i & 1)          /* odd -> subtract */
                cache[i] = t - last;
            else                /* even -> add           */
                cache[i] = t + last;
        }

        cache_used++;
    }

    return cache[n];
}
