/*
 * A013621  Triangle of coefficients in expansion of (2+5*x)^n.
 * https://oeis.org/A013621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = binomial(r,j) * 5^j * 2^(r-j) where r =
 *            floor((sqrt(8*n+1)-1)/2) (row index) j = n - r*(r+1)/2 (column
 *            index in the row)
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* --- find the row r and column j for index n -------------------- */
    long double m = sqrtl((long double)n * 8.0L + 1.0L);
    idx_t r = (idx_t)((m - 1.0L) / 2.0L);              /* initial guess */

    /* correct rounding issues that may arise from floating point */
    idx_t prefix = r * (r + 1) / 2;
    while (prefix > n) {        /* step down until the prefix fits   */
        r--;
        prefix = r * (r + 1) / 2;
    }
    while ((r + 1) * (r + 2) / 2 <= n) { /* or step up if needed      */
        r++;
    }
    idx_t j = n - r * (r + 1) / 2;                 /* column index      */

    /* --- compute the binomial coefficient binomial(r,j) --------------- */
    seq_t bin = 1;
    for (idx_t k = 1; k <= j; ++k) {
        bin *= (r - k + 1);   /* multiply by (r - k + 1)   */
        bin /= k;             /* divide by k (exact)        */
    }

    /* --- multiply by 5^j and 2^(r-j) -------------------------------- */
    seq_t result = bin;
    for (idx_t k = 0; k < j; ++k)   result *= 5;
    for (idx_t k = 0; k < r - j; ++k) result *= 2;

    return result;
}
