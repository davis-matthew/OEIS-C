/*
 * A227984  Triangle T(n,k), read by rows: T(n,k) is the numerator of
 *          (1-2^(n-k+1))/(1-2^(k+1)).
 * https://oeis.org/A227984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   a(n) = numerator of (1-2^(n-k+1))/(1-2^(k+1)) for the triangular
 *            array read by rows, where n = r*(r+1)/2 + k, 0 ≤ k ≤ r
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 5150, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility to compute 2^g in seq_t (big_pow expects seq_t exponent). */
#include "../utils/big_pow.h"

/* Return row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    /* Since n is small (≤ 10000) a linear scan is fast enough. */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

/* Euclidean algorithm for idx_t. */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Compute (r, k) for index n. */
    idx_t r = row_of(n);
    idx_t k = n - r * (r + 1) / 2;

    /* Exponents of two in numerator and denominator. */
    idx_t expN = r - k + 1;     /* n-k+1 */
    idx_t expD = k + 1;         /* k+1 */

    /* g = gcd(expN, expD). */
    idx_t g = gcd_idx(expN, expD);

    /* Number of terms in the geometric sum. */
    idx_t m = expN / g;

    /* Step size = 2^g. */
    seq_t step = big_pow((seq_t)2, (seq_t)g);

    /* Compute the numerator as a sum of powers of step. */
    seq_t sum = 0;
    seq_t cur = 1;
    for (idx_t i = 0; i < m; ++i)
    {
        sum += cur;
        cur *= step;
    }

    return sum;
}
