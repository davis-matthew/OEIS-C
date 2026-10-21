/*
 * A054375  Table of resultants for Chebyshev polynomials T_k(x) and T_n(x).
 * https://oeis.org/A054375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice, tabl
 * Formula:   a(n) = Resultant of Chebyshev polynomials T_k(x) and T_j(x),
 *            where the pair (k, j) is obtained by enumerating the
 *            upper-triangular table of resultants in row‑wise order. The
 *            resultant is zero iff both k/d and j/d are odd, where d = gcd(k,
 *            j); otherwise it equals ±2^e with e = (k‑1)(j‑1) + d – 1 T...
 * Range:     i8 n <= 13, i16 n <= 18, i32 n <= 33, i64 n <= 52, i128 n <= 89,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Find the row k such that k(k+1)/2 ≥ n */
    idx_t k = 1;
    while ((k * (k + 1)) / 2 < n) ++k;

    /* Column index j within the row */
    idx_t prev = (k - 1) * k / 2;
    idx_t j = n - prev;

    /* Check for zero resultant */
    idx_t d = gcd_idx(k, j);
    if (((k / d) & 1ULL) && ((j / d) & 1ULL))
        return (seq_t)0;

    /* Exponent of 2 in the magnitude */
    idx_t exp = (k - 1) * (j - 1) + d - 1;

    /* Count how many cos(j * n * (2*t - 1)π/(2k)) are negative
       This is done by checking the remainder modulo 4k. */
    idx_t neg = 0;
    for (idx_t t = 1; t <= k; ++t) {
        idx_t rem = (j * (2 * t - 1)) % (4 * k);
        if (rem > k && rem < 3 * k)
            ++neg;
    }

    /* Compute the result as ±2^exp */
    seq_t val = pow2(exp);
    if (neg & 1ULL)
        val = -val;
    return val;
}
