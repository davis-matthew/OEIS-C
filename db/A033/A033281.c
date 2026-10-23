/*
 * A033281  Number of diagonal dissections of a convex (n+9)-gon into n+1
 *          regions.
 * https://oeis.org/A033281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6,6)*binomial(2*n+8,n)/(n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t binom_multiplicative(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);
        seq_t g = gcd(result, (seq_t)i);
        result /= g;
        seq_t i_over = (seq_t)(i / (idx_t)g);
        result *= num;
        result /= i_over;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t binA = binom_multiplicative(n + 6, 6);
    seq_t binB = binom_multiplicative((idx_t)2 * n + 8, n);
    seq_t denom = (seq_t)(n + 1);

    seq_t g = gcd(binB, denom);
    binB /= g;
    denom /= g;

    g = gcd(binA, denom);
    binA /= g;
    denom /= g;

    seq_t result = binA * binB;
    if (denom != 1)
        result /= denom;
    return result;
}
