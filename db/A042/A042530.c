/*
 * A042530  Numerators of continued fraction convergents to sqrt(794).
 * https://oeis.org/A042530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Numerators of convergents to sqrt(794).
   Let a0 = floor(sqrt(794)) = 28.
   Let the continued‑fraction coefficients be c(0)=a0, c(1), c(2), ...
   The numerators satisfy p(-2)=0, p(-1)=1,
   p(k) = c(k)*p(k-1)+p(k-2)   for k ≥ 0.
   The function returns p(n). */

#include <stdint.h>

/* Return the n-th term of OEIS A042530. */
seq_t a(idx_t n)
{
    const idx_t N = 794;
    const idx_t a0 = 28;           /* floor(sqrt(794)) */

    idx_t m = 0;
    idx_t d = 1;
    idx_t a_coef = a0;

    seq_t p_minus2 = 0;   /* p(-2) */
    seq_t p_minus1 = 1;   /* p(-1) */
    seq_t p;

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute numerator for current k */
        p = (seq_t)a_coef * p_minus1 + p_minus2;

        if (k == n)
            return p;

        /* Shift indices for next step */
        p_minus2 = p_minus1;
        p_minus1 = p;

        /* Generate next coefficient of the continued fraction */
        m = d * a_coef - m;
        d = (N - m * m) / d;
        a_coef = (a0 + m) / d;
    }

    /* Unreachable, but keeps the compiler happy. */
    return 0;
}
