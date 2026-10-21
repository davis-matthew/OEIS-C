/*
 * A055858  Coefficient triangle for certain polynomials.
 * https://oeis.org/A055858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) comes from the coefficient triangle a(n,m) = 0 if n<m
 *            a(0,0) = 1 a(n,0) = n^n (n>=1) a(n,m) = n^(m-1)*(n+1)^(n-m+1)
 *            (n>=m>=1) The sequence a(n) is obtained by listing the
 *            coefficients in order of rows (row 0, row 1, …) and within each
 *            row by increasing column index. Thus a(n) = a(r,m) where...
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External helper: big integer power */
#include "../utils/big_pow.h"

/* Compute the sequence term */
seq_t a(idx_t n)
{
    /* Find the row index r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
       For n up to about 10000 the number of iterations is < 200. */
    idx_t r = 0;
    while ( ((__uint128_t)(r + 1) * (r + 2)) / 2 <= n )
        r++;

    /* Position inside the row */
    idx_t m = n - ((__uint128_t)r * (r + 1)) / 2;

    /* Special case: first row */
    if (r == 0)
        return (seq_t)1;

    /* First element of a row: r^r */
    if (m == 0)
        return big_pow((seq_t)r, (seq_t)r);

    /* General case: r^(m-1) * (r+1)^(r-m+1) */
    seq_t left  = big_pow((seq_t)r,       (seq_t)(m - 1));
    seq_t right = big_pow((seq_t)(r + 1), (seq_t)(r - m + 1));
    return left * right;
}
