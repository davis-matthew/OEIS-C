/*
 * A038295  Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*5^j.
 * https://oeis.org/A038295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 9^(i-j) * 5^j * where (i,j) are the row
 *            and column indices satisfying * i = floor((sqrt(8n+1)-1)/2) * j
 *            = n - i*(i+1)/2 * 0 <= j <= i * The sequence is obtained by
 *            concatenating the rows of the triangle * whose (i,j)-th entry is
 *            binomial(i,j)*9^(i-j)*5^j, offset 0
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes 5^e or 9^e for any non-negative exponent e using exponentiation by squaring. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= (idx_t)1;
    }
    return result;
}

/* Computes binomial coefficient C(n,k) for non-negative n and k with n<=64-bit. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        res = res * (n - k + i);
        res = res / i;  /* exact division */
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine the row i that contains the n-th term.  Since the
       total number of terms up to row i (0‑based) is (i+1)(i+2)/2,
       we find the smallest i such that this exceeds n. */
    idx_t i = (idx_t)0;
    while (((idx_t)(i + 1) * (i + 2)) / (idx_t)2 <= n)
        ++i;

    /* Determine the column j within that row. */
    idx_t j = n - (i * (i + 1)) / (idx_t)2;

    /* Compute the triangle entry: binomial(i,j) * 9^(i-j) * 5^j. */
    seq_t term = binom(i, j);
    term = term * pow_seq((seq_t)9, (idx_t)(i - j));
    term = term * pow_seq((seq_t)5, j);

    return term;
}
