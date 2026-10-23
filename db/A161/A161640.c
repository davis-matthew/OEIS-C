/*
 * A161640  Number of reduced words of length n in the Weyl group A_32.
 * https://oeis.org/A161640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{32} (1 + x + … + x^k)
 *            (product of 32 finite geometric series). Only finitely many
 *            terms are non‑zero; the degree is Σ_{k=1}^{32} k = 528
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <string.h>   /* for memset */

static seq_t coeff[529];          /* coefficient array for 0 … 528 */
static int coeff_initialized = 0; /* flag */

/* initialise the coefficient array once */
static void init_coeff(void)
{
    static seq_t tmp[529];
    size_t cur_deg = 0;            /* current degree of the product */

    memset(coeff, 0, sizeof(coeff));
    coeff[0] = 1;                  /* (product of zero factors) */

    for (idx_t k = 1; k <= 32; ++k)        /* multiply by (1 + x + … + x^k) */
    {
        memset(tmp, 0, sizeof(tmp));

        /* convolution: coeff ⊗ [1,1,…,1] (k+1 terms) */
        for (idx_t i = 0; i <= cur_deg; ++i)
        {
            seq_t v = coeff[i];
            if (!v) continue;      /* skip zero entries */
            for (idx_t j = 0; j <= k; ++j)
                tmp[i + j] += v;
        }

        /* copy the new coefficients back */
        cur_deg += k;
        memcpy(coeff, tmp, (cur_deg + 1) * sizeof(seq_t));
    }

    coeff_initialized = 1;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (!coeff_initialized)
        init_coeff();

    if (n <= 528)          /* polynomial degree is 528 */
        return coeff[n];
    else
        return 0;          /* coefficients beyond the degree are zero */
}
