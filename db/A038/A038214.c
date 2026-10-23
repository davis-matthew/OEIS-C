/*
 * A038214  Triangle whose (i,j)-th entry is binomial(i,j)*2^(i-j)*8^j.
 * https://oeis.org/A038214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) = binomial(r,k) * 2^(r-k) * 8^k, where r =
 *            floor((√(8*n+1)-1)/2) is the row number, and k = n - r*(r+1)/2
 *            is the column index
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial coefficient using only integer
   arithmetic that remains exact for values that fit in seq_t. */
static seq_t binomial_mul(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        result = result * (n - k + i);   /* multiply by the next factor */
        result = result / i;             /* divide by i, exact division */
    }
    return result;
}

/* integer power: result = base^exp  (exp <= 200 for all cases here) */
static seq_t pow_int_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result = result * base;
    return result;
}

seq_t a(idx_t n)
{
    /* find row r and column k by incremental counting */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum + r + 1 <= n)
    {
        sum += r + 1;
        ++r;
    }
    idx_t k = n - sum;

    /* compute the triangle entry T(r,k) = binomial(r,k)*2^(r-k)*8^k */
    seq_t val = binomial_mul(r, k);
    val = val * pow_int_seq((seq_t)2, r - k);
    val = val * pow_int_seq((seq_t)8, k);

    return val;
}
