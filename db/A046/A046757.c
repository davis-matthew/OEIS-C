/*
 * A046757  Triangle of coefficients of certain polynomials (exponents in
 *          decreasing order).
 * https://oeis.org/A046757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) flattened from the triangle where a(n,m) = (1+n^2) * C(n,m)
 *            * n^(n-m-2) for n>m>=0, a(n,n)=1
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include necessary utilities */
#include <stdbool.h>
#include <math.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

/* Computes the OEIS sequence A046757 value for a given index n. */
seq_t a(idx_t n)
{
    /* Determine the row r and column c (0‑based) in the triangle. */
    idx_t r = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    idx_t before = r * (r + 1) / 2;
    idx_t c = n - before;

    /* On the diagonal the value is always 1. */
    if (c == r)
        return (seq_t)1;

    /* Compute components of the formula. */
    seq_t r_seq = (seq_t)r;
    seq_t coeff = (seq_t)1 + r_seq * r_seq;          /* 1 + n^2 */
    seq_t bin   = binomial(r, c);                    /* C(n, m) */

    /* Exponent: n - m - 2.  It may be negative for m > n-2. */
    int diff = (int)r - (int)c - 2;

    seq_t result;
    if (diff >= 0) {
        /* Positive exponent: multiply by n^diff. */
        seq_t pow = pow_seq(r_seq, diff);
        result = coeff * bin * pow;
    } else {
        /* Negative exponent: divide by n^(-diff).  The division is exact. */
        seq_t denom    = pow_seq(r_seq, -diff);
        seq_t numerator = coeff * bin;
        result = numerator / denom;
    }

    return result;
}
