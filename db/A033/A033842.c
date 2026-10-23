/*
 * A033842  Triangle of coefficients of certain polynomials (exponents in
 *          decreasing order).
 * https://oeis.org/A033842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n, m) = binomial(n+1, m)*(n+1)^(n-m-1) (for the triangle); the
 *            flattened sequence a(n) is obtained by traversing the triangle
 *            row by row
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 65, u64 n <= 152,
 *            u128 n <= 405, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static idx_t find_row(idx_t n)
{
    idx_t r = 0;
    while (((__uint128_t)r * (r + 1)) / 2 <= n)
        r++;
    return r - 1;
}

/* Integer exponentiation by squaring for type seq_t.  Exponent < 64k
   for the problem constraints, so the loop is very short. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row r such that n lies in row r of the triangle. */
    idx_t r = find_row(n);

    /* Compute the column index m within that row. */
    idx_t row_start = r * (r + 1) / 2;
    idx_t m = n - row_start;   /* 0 ≤ m ≤ r */

    /* The last element of each row equals 1. */
    if (m == r)
        return (seq_t)1;

    /* Compute (r+1)^(r-m-1). */
    seq_t base = (seq_t)(r + 1);
    idx_t exp  = r - m - 1;
    seq_t pow  = pow_seq(base, exp);

    /* Compute binomial(r+1, m). */
    seq_t coeff = binomial(r + 1, m);

    return coeff * pow;
}
