/*
 * A162209  Number of reduced words of length n in the Weyl group D_6.
 * https://oeis.org/A162209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Polynomial: f(6)^2 * f(2) * f(4) * f(8) * f(10),
 * where f(m) = (1 - x^m)/(1 - x) = 1 + x + ... + x^{m-1}.
 * The coefficient of x^n equals the number of integer tuples  
 * (a,b,c,d,e,f) with 0≤a,b≤5, 0≤c≤1, 0≤d≤3, 0≤e≤7, 0≤f≤9
 * that satisfy a+b+c+d+e+f = n.
 */
#include <stddef.h>
#include <stdbool.h>

/* precomputed coefficients of the polynomial, size 31 (degree 30) */
static seq_t coeff[31] = {0};
static bool coeff_computed = false;

/* Compute the coefficients once */
static void compute_coeff(void)
{
    if (coeff_computed) return;

    /* iterate over all valid tuples and increment the corresponding
     * coefficient. The ranges are small, so this is trivial. */
    for (idx_t a = 0; a < 6; ++a)
        for (idx_t b = 0; b < 6; ++b)
            for (idx_t c = 0; c < 2; ++c)
                for (idx_t d = 0; d < 4; ++d)
                    for (idx_t e = 0; e < 8; ++e)
                        for (idx_t f = 0; f < 10; ++f) {
                            idx_t sum = a + b + c + d + e + f;
                            coeff[sum] += (seq_t)1;
                        }

    coeff_computed = true;
}

seq_t a(idx_t n)
{
    compute_coeff();

    if (n < 31)
        return coeff[n];

    return (seq_t)0;
}
