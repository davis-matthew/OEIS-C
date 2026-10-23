/*
 * A162204  Number of reduced words of length n in the Weyl group B_49.
 * https://oeis.org/A162204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. for B_49 is ∏_{k=1}^{49} (1 - x^{2k})/(1-x)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* maximum degree of the polynomial for B_49: 49^2 = 2401 */
#define MAX_DEG 2401

/* Pre‑computed coefficients of the generating polynomial. */
static seq_t coeff[MAX_DEG + 1];
/* indicate whether the coefficients have been initialised */
static unsigned init_done = 0;

/* Initialise the coefficient array once on first call. */
static void init_coeff(void)
{
    if (init_done)
        return;

    /* coeff[0] = 1 initially. Remaining entries are 0 by static init. */
    coeff[0] = 1;

    /* Temporary arrays used during construction. */
    seq_t tmp[MAX_DEG + 1];
    seq_t prefix[MAX_DEG + 1];

    /* current degree of the polynomial */
    idx_t deg = 0;

    /* For each factor (1 + x + … + x^{2k-1}) we convolve
       the current polynomial with a “run‑length” kernel. */
    for (idx_t k = 1; k <= 49; ++k) {
        idx_t window = 2 * k - 1;            /* size of the kernel */
        idx_t new_deg = deg + window;        /* new degree after multiplication */

        /* Build prefix sums of the present polynomial.          */
        /* prefix[i] = coeff[0] + … + coeff[i]   for 0 ≤ i ≤ deg */
        if (deg == 0) {
            prefix[0] = coeff[0];
        } else {
            prefix[0] = coeff[0];
            for (idx_t i = 1; i <= deg; ++i)
                prefix[i] = prefix[i - 1] + coeff[i];
        }

        /* Compute new coefficients tmp[0..new_deg]. */
        for (idx_t n = 0; n <= new_deg; ++n) {
            idx_t high = (n <= deg) ? n : deg;       /* upper bound in old polynomial */
            idx_t low  = (n >= window) ? n - window : 0; /* lower bound for summation */

            seq_t s = prefix[high];
            if (low > 0)
                s = s - prefix[low - 1];
            tmp[n] = s;
        }

        /* Copy new coefficients back to coeff[]. */
        for (idx_t i = 0; i <= new_deg; ++i)
            coeff[i] = tmp[i];

        /* Update current degree. */
        deg = new_deg;
    }

    init_done = 1;
}

/* Return a(n) : the number of reduced words of length n in B_49. */
seq_t a(idx_t n)
{
    init_coeff();

    if (n <= MAX_DEG)
        return coeff[(idx_t)n];
    else
        return 0;
}
