/*
 * A075513  Triangle read by rows. T(n, m) are the coefficients of Sidi
 *          polynomials.
 * https://oeis.org/A075513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(r,m) with
 *            T(r,m)=(-1)^(r-m-1)*binomial(r-1,m)*(m+1)^(r-1) read by rows
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A075513 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

/* Find the row r of the triangle such that r*(r+1)/2 >= n */
static idx_t
find_row(idx_t n)
{
    idx_t low  = 1;
    idx_t high = n;          /* r never exceeds n */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        idx_t sum = (idx_t)(prod / 2);
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;           /* undefined for n < 1, defensive guard */

    /* Determine row r and column m (0‑based) within that row */
    idx_t r = find_row(n);

    unsigned __int128 prod_row = (unsigned __int128)r * (r - 1);
    idx_t prefix = (idx_t)(prod_row / 2);            /* sum of terms in previous rows */
    idx_t m = n - prefix - 1;                        /* column index in the row */

    /* Compute the binomial coefficient and the power */
    seq_t bin = binomial(r - 1, m);
    seq_t pow = seq_pow((seq_t)(m + 1), r - 1);

    /* Combine with the parity sign */
    seq_t res = bin * pow;
    if (((r - m - 1) & 1) != 0)
        res = -res;

    return res;
}
