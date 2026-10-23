/*
 * A162378  Number of reduced words of length n in the Weyl group D_31.
 * https://oeis.org/A162378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{m∈{31,2,4,…,60}} (1+x+…+x^{m-1})
 *            i.e. the product of the polynomials f(m) = (1‑x^m)/(1‑x)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Global storage for the full product polynomial */
static seq_t *poly_cache = NULL;
static idx_t poly_deg = 0;
static bool init = false;

/* Compute the product polynomial once (degree 930). */
static void compute_polynomial(void)
{
    /* List of m values in the order specified. */
    static const idx_t m_list[31] = {
        31,
        2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28,
        30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54,
        56, 58, 60
    };

    /* Start with the polynomial 1. */
    poly_deg = 0;
    poly_cache = (seq_t *)malloc((poly_deg + 1) * sizeof(seq_t));
    poly_cache[0] = (seq_t)1;

    /* Convolve with each (1 + x + … + x^{m-1}) polynomial. */
    for (size_t f = 0; f < 31; ++f)
    {
        idx_t m = m_list[f];
        idx_t new_deg = poly_deg + (m - 1);
        seq_t *new_poly = (seq_t *)malloc((new_deg + 1) * sizeof(seq_t));

        seq_t s = 0;          /* running window sum */
        for (idx_t i = 0; i <= new_deg; ++i)
        {
            if (i <= poly_deg)
                s += poly_cache[i];
            if (i >= m)
                s -= poly_cache[i - m];
            new_poly[i] = s;
        }

        free(poly_cache);
        poly_cache = new_poly;
        poly_deg   = new_deg;
    }
}

/* Public function: sequence a(n). */
seq_t a(idx_t n)
{
    if (!init)
    {
        compute_polynomial();
        init = true;
    }

    return (n <= poly_deg) ? poly_cache[n] : (seq_t)0;
}
