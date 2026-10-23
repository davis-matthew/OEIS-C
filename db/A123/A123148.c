/*
 * A123148  Triangle read by rows: T(n,k) is the coefficient of x^k in the
 *          polynomial p(n,x) defined by p(0,x) = -1, p(1,x) = x-2, p(n,x) =
 *          -x*p(n-1,x) + 2*p(n-2,x) for n >= 3 and 0 <= k <= n.
 * https://oeis.org/A123148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (-1)^(k+1) * 2^⌊(r-k+1)/2⌋ * C(⌊(r+k)/2⌋, k) where r =
 *            ⌊(√(8·n + 1) - 1)/2⌋ and k = n - r·(r + 1)/2
 * Range:     i8 n <= 46, i16 n <= 174, i32 n <= 603, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include utilities for binomial coefficients and powers of two */
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* External linkage required by the build system */
seq_t a(idx_t n)
{
    /* Find the row index r such that n lies in row r */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Offset within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute the power of two part: 2^⌊(r-k+1)/2⌋ */
    idx_t exp = (r - k + 1) / 2;               /* floor division */
    seq_t val = pow2(exp);

    /* Compute the binomial part: C(⌊(r + k)/2⌋, k) */
    idx_t m   = (r + k) / 2;                  /* floor division */
    seq_t bin = binomial(m, k);

    /* Multiply by the binomial coefficient */
    val = val * bin;

    /* Apply the sign: (-1)^(k+1) is negative when k is even */
    if ((k & 1) == 0)
        val = -val;

    return val;
}
