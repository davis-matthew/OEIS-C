/*
 * A101022  Table of numerators of coefficients of certain rational
 *          polynomials.
 * https://oeis.org/A101022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   binom, find_row, gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A101022 – numerators of coefficients of rational polynomials
    a(n) = numerator of the coefficient R(r,m) where
           r = smallest integer with r*(r+1)/2 ≥ n
           m = n – (r‑1)*r/2 – 1   (0‑based column in row r)
    R(r,m) = binom(r‑1,m) * 2^m
            ------------------------------------
            (m+1)*(2*m+1)*binom(2*m,m)
    The returned value is the reduced numerator.  */

#include <sys/types.h>   /* for idx_t and seq_t types used by the build */
#include "../utils/binom.h"
#include "../utils/find_row.h"
#include "../utils/gcd_seq.h"

/* Euclidean algorithm for types that support % */

/* Integer power 2^exp for small exponents (exp ≤ 200) */
static seq_t pow2(idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)2;
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n = 0 – keep safe */

    /* Determine the row r and the 0‑based column m */
    idx_t r = find_row(n);
    idx_t prev = (r - 1) * r / 2;
    idx_t m = n - prev - 1;       /* 0 ≤ m < r */

    /* Compute numerator and denominator of R(r,m) */
    seq_t num = binom(r - 1, m) * pow2(m);
    seq_t den = (seq_t)(m + 1);
    den *= (seq_t)(2 * m + 1);
    den *= binom(2 * m, m);

    /* Reduce fraction and return numerator */
    seq_t g = gcd_seq(num, den);
    return num / g;
}
