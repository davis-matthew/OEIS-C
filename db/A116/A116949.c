/*
 * A116949  Riordan array ((1-x^3)/(1+2x^2),x).
 * https://oeis.org/A116949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the (r-k)th coefficient of (1-x^3)/(1+2*x^2), * where r
 *            = floor((sqrt(8n+1)-1)/2) is the row of the flattened * Riordan
 *            array ((1-x^3)/(1+2*x^2),x) and k = n - r(r+1)/2 is the *
 *            position within that row. Thus a(n) = d_{r-k} where * * d_m = 1
 *            if m = 0 * d_m = (-2)^(m/2) if m even, m > 0 *...
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   d_coeff, pow_minus_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/d_coeff.h"
#include "../utils/pow_minus_two.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Determine row r: smallest r such that r*(r+1)/2 > n,
       i.e., T(r) <= n < T(r+1).  Use a small loop because n <= ~10⁴
       in the expected tests. */
    idx_t r = 0;
    while (true) {
        /* Use 128-bit to prevent overflow for large values of r. */
        unsigned __int128 t_next = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (t_next > n)
            break;
        ++r;
    }

    /* Starting index of this row: T(r) = r*(r+1)/2. */
    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - start;              /* column index within the row */
    idx_t m = r - k;                  /* index in the coefficient sequence */

    return d_coeff(m);
}
