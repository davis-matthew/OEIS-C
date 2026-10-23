/*
 * A033779  Product t2(q^d); d | 20, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{d|20} ∑_{k≥0} q^{d*k(k+1)/2},
 *            where the divisors of 20 are 1,2,4,5,10,20
 * Range:     u8 n <= 99, u16 n <= 1526, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#  include <new>
#endif

static idx_t coeff_size = 0;          /* size of allocated coefficient array (entries 0 … size-1) */
static seq_t *coeff = NULL;           /* cached coefficients */

/* Allocate an array of seq_t, zero‑initialised, of length limit+1. */
static seq_t *alloc_zeroed(idx_t limit)
{
#ifdef __cplusplus
    seq_t *p = new seq_t[limit + 1];
    for (idx_t i = 0; i <= limit; ++i) p[i] = 0;
    return p;
#else
    return (seq_t*)calloc((size_t)limit + 1, sizeof(seq_t));
#endif
}

/* Free an array previously allocated by alloc_zeroed. */
static void free_array(seq_t *p)
{
    if (!p) return;
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Compute all coefficients up to and including 'limit', storing them in coeff. */
static void compute_up_to(idx_t limit)
{
    static const idx_t divs[] = {1, 2, 4, 5, 10, 20};
    const size_t num_divs = sizeof(divs) / sizeof(divs[0]);

    seq_t *a = alloc_zeroed(limit);
    a[0] = 1;                            /* start with the constant term */

    for (size_t vi = 0; vi < num_divs; ++vi)
    {
        idx_t d = divs[vi];
        seq_t *conv = alloc_zeroed(limit);

        /* accumulate contributions from the series of t₂(qᵈ)
           whose terms have exponents d * k(k+1)/2. */
        for (idx_t m = 0;; ++m)
        {
            idx_t tri = m * (m + 1) / 2;          /* triangular number k(k+1)/2 */
            if (tri > limit / d) break;          /* further terms would exceed limit */
            idx_t e = d * tri;                   /* exponent of q in this term */

            for (idx_t i = 0; i + e <= limit; ++i)
            {
                if (a[i] != 0)
                    conv[i + e] += a[i];
            }
        }

        free_array(a);
        a = conv;   /* a now holds the partial product */
    }

    free_array(coeff);
    coeff = a;
    coeff_size = limit + 1;
}

seq_t a(idx_t n)
{
    /* extend cache if necessary */
    if (coeff == NULL || n >= coeff_size)
        compute_up_to(n);
    return coeff[n];
}
