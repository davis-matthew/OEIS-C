/*
 * A161933  Number of reduced words of length n in the Weyl group B_26.
 * https://oeis.org/A161933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in \prod_{k=1}^{26} (1 + x + x^2 + … +
 *            x^{2k-1})
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 49, u128 n <= 676,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product can be expanded incrementally by multiplying
   the current polynomial by (1 + x + … + x^{2k-1}) for k = 1..26.
   The result is a polynomial of maximum degree 26^2 = 676. */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

#define M      26U
#define MAX_DEG ((size_t)(M*M))

static seq_t *coeff = NULL;   /* holds the coefficients a(0)…a(MAX_DEG) */

/* Compute the polynomial once and cache the coefficients. */
static void init_coefficients(void)
{
    size_t i, s, step, new_deg, curr_deg = 0;
    seq_t *tmp;

    /* Allocate array for the final result. */
    coeff = alloc_seq_array(MAX_DEG + 1);
    for (i = 0; i <= MAX_DEG; ++i) coeff[i] = 0;
    coeff[0] = 1;    /* start with constant polynomial 1 */

    for (size_t k = 1; k <= M; ++k)
    {
        step = 2 * k;                 /* factor 1 + x + … + x^{2k-1} has "step" terms */
        new_deg = curr_deg + step - 1;/* (+1 because degree = terms-1)     */

        tmp = alloc_seq_array(new_deg + 1);
        for (i = 0; i <= new_deg; ++i) tmp[i] = 0;

        /* Convolve current polynomial with the new factor. */
        for (i = 0; i <= curr_deg; ++i)
        {
            seq_t val = coeff[i];
            for (s = 0; s < step; ++s)
                tmp[i + s] += val;
        }

        free_seq_array(coeff);
        coeff = tmp;
        curr_deg = new_deg;
    }
}

/* Return a(n) for any non‑negative n. */
seq_t a(idx_t n)
{
    if (coeff == NULL)                 /* lazy initialization */
        init_coefficients();

    if (n > (idx_t)MAX_DEG) /* terms beyond degree MAX_DEG are zero */
        return 0;

    return coeff[(size_t)n];
}
