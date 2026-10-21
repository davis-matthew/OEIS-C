/*
 * A084604  Triangle, read by rows, where the n-th row lists the (2n+1)
 *          coefficients of (1 + x + 4x^2)^n.
 * https://oeis.org/A084604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th entry of the triangle containing the
 *            coefficients of (1 + x + 4x^2)^k read by rows. Row k (k ≥ 0) has
 *            2k+1 columns; for index n we have k = ⌊√n⌋, t = n – k² and a(n)
 *            is the coefficient of x^t in (1 + x + 4x^2)^k
 * Range:     u8 n <= 21, u16 n <= 72, u32 n <= 213, u64 n <= 762,
 *            u128 n <= 2600, big any n
 * Helpers:   binomial, floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_isqrt_u64.h"
#include "../utils/binomial.h"

static seq_t coeff(idx_t k, idx_t e)
{
    seq_t result = 0;
    idx_t start   = (e > k) ? (e - k) : 0;   /* minimal c₂ */
    idx_t max_c2  = e / 2;                   /* maximal c₂ */

    seq_t pow4 = 1;
    for (idx_t i = 0; i < start; ++i)
        pow4 = pow4 * (seq_t)4;

    for (idx_t c2 = start; c2 <= max_c2; ++c2) {
        idx_t c1 = e - 2 * c2;               /* number of x terms */
        idx_t c0 = k - c1 - c2;              /* number of 1 terms */

        seq_t term = binomial(k, c0);
        term = term * binomial(k - c0, c1);
        term = term * pow4;
        result = result + term;
        pow4 = pow4 * (seq_t)4;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t k = floor_isqrt_u64(n);
    idx_t t = n - k * k;
    return coeff(k, t);
}
