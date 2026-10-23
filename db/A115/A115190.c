/*
 * A115190  Fourth diagonal (M=4) of triangle A115154 (called Y(3,1)).
 * https://oeis.org/A115190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of xⁿ in * G(x) =
 *            ((-3+18x+2x³)+3(1-9x+9x²)c(3x))/((2+x)x³), * where c(x) is the
 *            Catalan generating function. * Derived recurrence (for n≥0): *
 *            a(n) + 2·a(n+1) = coeff_N(n+4) * with * coeff_N(m) = 3^{m+1}(C_m
 *            + C_{m-2}) – 3^{m+2}C_{m-1}, * C_k = Catalan(k). * Therefore *
 *            a(n+1)...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* current value a(k), starting with a(0) */
    seq_t a_prev = (seq_t)1;

    /* 3⁴ = 81 – first power needed for m = 4 */
    seq_t pow3 = (seq_t)81;

    for (idx_t k = (idx_t)0; k < n; ++k) {
        /* m = k + 4  (coefficients start at m = 4) */
        idx_t m = k + (idx_t)4;

        /* Catalan numbers needed: C_m, C_{m-1}, C_{m-2} */
        seq_t cat_m   = catalan(m);
        seq_t cat_m1  = catalan(m - (idx_t)1);
        seq_t cat_m2  = catalan(m - (idx_t)2);

        /* coeff_N(m) = 3^{m+1}(C_m + C_{m-2}) – 3^{m+2}C_{m-1} */
        seq_t term1 = pow3 * ((seq_t)3) * (cat_m + cat_m2);
        seq_t term2 = pow3 * ((seq_t)9) * cat_m1;
        seq_t coeff = term1 - term2;

        /* a(k+1) = (coeff - a(k)) / 2 */
        seq_t a_next = (coeff - a_prev) / ((seq_t)2);

        /* Prepare for next iteration */
        a_prev = a_next;
        pow3   *= (seq_t)3;            /* pow3 now equals 3^{m+1} */
    }

    return a_prev;
}
