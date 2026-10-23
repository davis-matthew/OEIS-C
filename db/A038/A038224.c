/*
 * A038224  Triangle whose (i,j)-th entry is binomial(i,j)*3^(i-j)*6^j.
 * https://oeis.org/A038224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) = binomial(i,j) * 3^(i-j) * 6^j, where i = floor((sqrt(1 +
 *            8n) - 1)/2) and j = n - i(i+1)/2; the triangle is read row‑wise
 *            (row 0, row 1, …)
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the helper function _floor_isqrt_u64 is available but is not used here
   to avoid overflow when computing 1+8*n.  Instead, a floating point
   approximation is refined with integer arithmetic. */

#include <math.h>    /* sqrtl is used for the floating point approximation */

seq_t a(idx_t n)
{
    /* 1. Find row index i such that i(i+1)/2 <= n < (i+1)(i+2)/2
       – use a floating‑point approximation and correct with a few
         integer steps to guarantee exactness. */
    long double approx = sqrtl((long double)1 + (long double)8 * (long double)n);
    idx_t i = (idx_t)((approx - 1.0L) / 2.0L);

    /* Correct any possible off‑by‑one errors due to rounding. */
    while (( (__int128)(i + 1) * (i + 2) ) / 2 <= n)
        ++i;
    while (( (__int128)i * (i + 1) ) / 2 > n)
        --i;

    /* 2. Column index j within the row. */
    idx_t j = n - (i * (i + 1)) / 2;

    /* 3. Compute binomial(i,j) multiplicatively. */
    seq_t binom = 1;
    for (idx_t r = 1; r <= j; ++r)
    {
        binom = binom * (i - j + r) / r;   /* division is exact */
    }

    /* 4. Compute powers of 3 and 6. */
    seq_t pow3 = 1;
    for (idx_t e = 0; e < i - j; ++e)
        pow3 *= (seq_t)3;

    seq_t pow6 = 1;
    for (idx_t e = 0; e < j; ++e)
        pow6 *= (seq_t)6;

    /* 5. Return the product. */
    return binom * pow3 * pow6;
}
