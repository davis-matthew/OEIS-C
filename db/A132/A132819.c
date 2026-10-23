/*
 * A132819  A001263 * A127773.
 * https://oeis.org/A132819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = product of two lower triangular matrices with entries
 *            given by A001263: binomial coefficients C(r,k) A127773: values
 *            computed by f(r,k) = k(k+1) / (2(k-1)) for k>1, f(r,1)=1 The
 *            linear index n (offset = 1) is obtained by listing the triangle
 *            row by row. This implementation finds the (r,k)...
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Utility functions provided by the build system */
#include "../utils/binom.h"  /* seq_t binom(idx_t a, idx_t b) */
#include "../utils/gcd.h"    /* seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for offset = 1 */

    /* Find the row r such that T(r‑1) < n ≤ T(r) with T(r)=r*(r+1)/2 */
    idx_t r = 1;
    idx_t triangular = 1; /* T(1) */
    while (triangular < n) {
        ++r;
        triangular = r * (r + 1) / 2;
    }

    /* Previous triangular number for row r‑1 */
    idx_t prev_tri = (r - 1) * r / 2;

    /* Column index k (1‑based) */
    idx_t k = n - prev_tri;

    /* a(r,1) is always 1 */
    if (k <= 1)
        return 1;

    /* Binomial factors */
    seq_t bin1 = binom(r, k);         /* C(r,k)   */
    seq_t bin2 = binom(r - 1, k - 2); /* C(r‑1,k‑2) */

    /* Coefficient f(r,k) = k(k+1) / (2(k-1)) */
    seq_t num = (seq_t)k * (seq_t)(k + 1);   /* numerator   */
    seq_t den = 2 * (k - 1);                 /* denominator */

    /* Simplify the denominator by cancelling with the binomial factors */
    seq_t g = gcd(bin1, den);
    if (g > 1) {
        bin1 /= g;
        den /= g;
    }

    g = gcd(bin2, den);
    if (g > 1) {
        bin2 /= g;
        den /= g;
    }

    /* Compute the final result: (bin1 * bin2 * num) / den */
    seq_t result = bin1;
    result *= bin2;
    result *= num;

    if (den != 1)
        result /= den;

    return result;
}
