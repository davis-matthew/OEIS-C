/*
 * A225564  Expansion of psi(-x)^2 * f(-x^4)^6 in powers of x where psi(), f()
 *          are Ramanujan theta functions.
 * https://oeis.org/A225564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 41, i16 n <= 884, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A225564
 * Formula: a(n) = coeff of x^n in ∏_{k>0} (1 - x^(4*k))^8 * (1 - x^(2*k - 1))^2
 * Computation is performed by successive convolution with the sparse
 * polynomials (1 - 2x^(2k-1) + x^(4k-2)) * Σ_{j=0}^8 (-1)^j C(8,j) x^(4k*j).
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const seq_t binom8[9] = {1,8,28,56,70,56,28,8,1};

seq_t a(idx_t n)
{
    /* trivial case n = 0 */
    if (n == 0) return (seq_t)1;

    /* allocate coefficient arrays */
    const idx_t N = n;
    seq_t *coeff = (seq_t*)calloc(N+1, sizeof(seq_t));
    seq_t *next  = (seq_t*)calloc(N+1, sizeof(seq_t));
    if (!coeff || !next) return (seq_t)0;      /* fallback for OOM */

    coeff[0] = (seq_t)1;

    /* iterate over k */
    for (idx_t k = 1; ; ++k)
    {
        /* stop if the smallest exponent contributed by this k exceeds N */
        if ((2*k-1 > N) && (4*k > N)) break;

        /* clear the next array */
        memset(next, 0, (N+1)*sizeof(seq_t));

        /* precompute base exponents and coefficients */
        const idx_t exp_a = 2*k-1;
        const idx_t exp_b = 4*k-2;
        const seq_t coeff_a[3] = {1, (seq_t)-2, 1};
        const idx_t exp_base[3] = {0, exp_a, exp_b};

        /* iterate over j for the (1 - x^(4k))^8 factor */
        for (idx_t j = 0; j <= 8; ++j)
        {
            const seq_t coeff_j = (j & 1) ? -(seq_t)binom8[j] : binom8[j];
            const idx_t exp_j = 4*k*j;
            if (exp_j > N) continue;

            /* multiply with each base term */
            for (int m = 0; m < 3; ++m)
            {
                const idx_t exp = exp_j + exp_base[m];
                if (exp > N) continue;

                const seq_t coeff_p = coeff_a[m] * coeff_j;
                if (coeff_p == (seq_t)0) continue;

                /* convolution: for each t >= exp */
                for (idx_t t = exp; t <= N; ++t)
                {
                    next[t] += coeff[t - exp] * coeff_p;
                }
            }
        }

        /* swap coeff and next for next iteration */
        seq_t *tmp = coeff;
        coeff = next;
        next = tmp;
    }

    /* result */
    seq_t result = coeff[N];

    /* free memory */
    free(coeff);
    free(next);

    return result;
}
