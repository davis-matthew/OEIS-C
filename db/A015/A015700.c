/*
 * A015700  Inverse of 1691st cyclotomic polynomial.
 * https://oeis.org/A015700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series expansion of
 *            1/Φ_{1691}(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) by computing the cyclotomic polynomial Φ_{1691}(x) and using the   */
/* recurrence a(m) = -(c1*a(m-1)+c2*a(m-2)+...+cφ*a(m-φ)), where c_k are the          */
/* coefficients of Φ_{1691}(x). The sequence is periodic with period 1691.            */
#include <stdlib.h>
#include <stdint.h>

static const idx_t P = 19;          /* first prime factor of 1691 */
static const idx_t Q = 89;          /* second prime factor of 1691 */
static const idx_t DEG = (P-1)*(Q-1);/* degree of Φ_{1691} (phi(1691) = 1584) */

/* Compute coefficients of Φ_{1691}(x) (degree DEG) and store them in coeff[] */
static void compute_phi_coefficients(seq_t *coeff)
{
    /* coeff[0] = 1 */
    coeff[0] = (seq_t)1;
    for (idx_t t = 1; t <= DEG; ++t) {
        /* N_t = 1 if t is a multiple of Q and t/Q ≤ P-1, otherwise 0 */
        seq_t N_t = ( (t % Q == 0) && (t / Q <= P-1) ) ? (seq_t)1 : (seq_t)0;
        seq_t sum = (seq_t)0;
        idx_t max_i = (t < (P-1)) ? t : (P-1);
        for (idx_t i = 1; i <= max_i; ++i)
            sum += coeff[t - i];
        coeff[t] = N_t - sum; /* Q_t = N_t - sum_{i=1}^{P-1} Q_{t-i} */
    }
}

/* Return a(n) */
seq_t a(idx_t n)
{
    /* Lazy static storage for caching */
    static seq_t *coeff = NULL;      /* coefficients of Φ_{1691} */
    static seq_t *seq_cache = NULL;  /* computed a(m) values */
    static idx_t cache_size = 0;     /* allocated size of seq_cache */
    static idx_t computed_up_to = 0; /* next index to compute */

    /* Ensure coefficients are computed */
    if (!coeff) {
        coeff = (seq_t *)calloc((size_t)(DEG + 1), sizeof(seq_t));
        if (!coeff) abort();
        compute_phi_coefficients(coeff);
    }

    /* Expand cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        seq_cache = new_cache;
        cache_size = new_size;
    }

    /* Compute new values if needed */
    if (computed_up_to <= n) {
        if (computed_up_to == 0) {
            seq_cache[0] = (seq_t)1;
            computed_up_to = 1;
        }
        for (idx_t m = computed_up_to; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            idx_t limit = (m < DEG) ? m : DEG;
            for (idx_t k = 1; k <= limit; ++k)
                sum += coeff[k] * seq_cache[m - k];
            seq_cache[m] = -sum;
        }
        computed_up_to = n + 1;
    }

    return seq_cache[n];
}
