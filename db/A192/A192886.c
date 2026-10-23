/*
 * A192886  Expansion of x^4*(1 - x)^2*(1 - 3*x^2 - 2*x^3 + x^4 - x^5)/(1 -
 *          2*x)^3.
 * https://oeis.org/A192886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192886: Coefficients of
   x^4*(1 - x)^2*(1 - 3*x^2 - 2*x^3 + x^4 - x^5) / (1 - 2*x)^3.
   For n ≥ 12  a(n) = 2^(n-12) * (n^2 + 137*n - 234). */
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Small initial values (n = 0 … 11) */
    static const seq_t init[12] = {
        0, 0, 0, 0, /* a(0) … a(3) */
        1, 4, 10, 24, /* a(4) … a(7) */
        58, 135, 309, 697 /* a(8) … a(11) */
    };

    if (n < 12)
        return init[n];

    /* For n ≥ 12 use the closed form */
    seq_t pow_term = pow2(n - 12);                     /* 2^(n-12) */
    seq_t n_sq            = (seq_t)n * (seq_t)n;       /* n²  */
    seq_t linear          = (seq_t)137 * (seq_t)n;    /* 137*n */
    seq_t poly_term = n_sq + linear - (seq_t)234;     /* n² + 137*n - 234 */

    return pow_term * poly_term;
}
