/*
 * A208659  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A185045; see the Formula section.
 * https://oeis.org/A208659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the linearized triangle of coefficients T(r,k) of the *
 *            polynomial v(r,x) = 2*(1+x)*(1+2x)^(r-2). For a given index n
 *            (1‑based) * find the row r and the position k (0‑based) in that
 *            row, then compute * T(r,k) via the closed form * * T(r,k) = 2*(
 *            binom(r-2,k)*2^k + binom(r-2,k-1)*2^(k-1) )...
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r such that S(r-1) < n ≤ S(r), where S(r)=r(r+1)/2. */
    idx_t r = (idx_t)((sqrt((double)(1 + 8 * n)) - 1.0) / 2.0);
    /* Adjust r for possible rounding errors. */
    while ((r * (r + 1)) / 2 < n) ++r;
    while (r > 0 && ((r - 1) * r) / 2 >= n) --r;

    idx_t prevS = (r - 1) * r / 2;          /* S(r-1) */
    idx_t k = n - prevS - 1;                /* 0‑based position in row */

    if (r == 1) return (seq_t)1;            /* Special case: first row */

    /* Compute the two binomial terms. */
    seq_t term1 = binomial(r - 2, k) * pow2(k);
    seq_t sum = term1;
    if (k > 0) {
        seq_t term2 = binomial(r - 2, k - 1) * pow2(k - 1);
        sum = sum + term2;
    }

    /* Final result: multiply by 2. */
    return sum * (seq_t)2;
}
