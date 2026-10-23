/*
 * A162327  Number of reduced words of length n in the Weyl group D_16.
 * https://oeis.org/A162327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in (1 + x + … + x^15) * Π_{i=1}^{15}
 *            (1 + x + … + x^{2i-1})
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 21, u64 n <= 240, u128 n <= 240,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The polynomial has degree 240, so a(n) = 0 for n > 240. */
#include <stddef.h>
#include <stdbool.h>

static seq_t coeff[241];
static bool coeff_computed = false;

static void compute_coeffs(void)
{
    if (coeff_computed) return;
    /* f(16) and f(2i) for i = 1..15 (f(16) occurs twice, once for k=16 and once for i=8). */
    const idx_t factors[16] = {16, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
    const idx_t max_degree = 240;

    /* Start with polynomial 1. */
    coeff[0] = 1;
    idx_t size = 1;   /* highest degree + 1 */

    seq_t temp[241];

    for (idx_t f = 0; f < 16; ++f) {
        idx_t m = factors[f];
        idx_t new_size = size + m - 1;
        for (idx_t i = 0; i < new_size; ++i) temp[i] = 0;

        /* Convolution with 1 + x + … + x^{m-1} (all coefficients 1). */
        for (idx_t i = 0; i < size; ++i) {
            seq_t v = coeff[i];
            for (idx_t d = 0; d < m; ++d) {
                temp[i + d] += v;
            }
        }

        for (idx_t i = 0; i < new_size; ++i) coeff[i] = temp[i];
        size = new_size;
    }

    /* Zero remaining entries up to degree 240. */
    for (idx_t i = size; i <= max_degree; ++i) coeff[i] = 0;

    coeff_computed = true;
}

seq_t a(idx_t n)
{
    if (n > 240) return 0;
    compute_coeffs();
    return coeff[n];
}
