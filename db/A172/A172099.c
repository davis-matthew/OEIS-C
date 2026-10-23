/*
 * A172099  Irregular triangle T(n, k) = [x^k]( p(n, x) ), where p(n, x) =
 *          x^(2*n-1)*p(n-1, x) + p(n-2, x) with p(0, x) = 1 and p(1, x) = 1 +
 *          x, read by rows.
 * https://oeis.org/A172099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient [x^k] of the polynomial p(n,x) defined by
 *            p(0,x) = 1, p(1,x) = 1 + x, p(n,x) = x^(2*n-1) * p(n-1,x) +
 *            p(n-2,x) (n ≥ 2), where the linear index n is split into (row r,
 *            column k) by offset_row(r) = r + Σ_{i=0}^{r-1} i²
 * Range:     u8 n <= 2645, u16 n <= 9485, u32 n <= 9485, u64 n <= 9485,
 *            u128 n <= 9485, big any n
 * Helpers:   T_coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* standard integer types; seq_t and idx_t are defined by the build system */
#include <stddef.h>
#include "../utils/T_coeff.h"

/* ------------------------------------------------------------ */
/* offset of the first element of row r (0‑based)               */
static inline idx_t
offset_row(idx_t r)
{
    if (r == 0) return 0;
    /* r + ((r-1)*r*(2*r-1))/6  is always integer for r ≥ 1.   */
    return r + ((r - 1) * r * (2 * r - 1)) / 6;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* public interface:   seq_t a(idx_t n);                        */
seq_t a(idx_t n)
{
    /* find the row r that contains the linear index n */
    idx_t r = 0;
    while (offset_row(r + 1) <= n) {
        ++r;
    }

    /* column index within that row */
    idx_t k = n - offset_row(r);

    /* compute the required coefficient */
    return T_coeff(r, k);
}
