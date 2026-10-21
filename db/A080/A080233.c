/*
 * A080233  Triangle T(n,k) obtained by taking differences of consecutive
 *          pairs of row elements of Pascal's triangle A007318.
 * https://oeis.org/A080233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = C(r,c) - C(r,c-1) where r is the row such that r*(r+1)/2
 *            <= n < (r+1)*(r+2)/2, c = n - r*(r+1)/2. For c = 0 the value is
 *            always 1
 * Range:     i8 n <= 38, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static idx_t find_row(idx_t n)
{
    /* Find the largest r with r*(r+1)/2 <= n  (row index). */
    idx_t low  = 0;
    idx_t high = 1;

    /* Grow high until it is larger than needed (binary search range). */
    while (( (__int128) high * (high + 1) / 2 ) <= n)
        high <<= 1;       /* high*2 each iteration */

    /* Binary search in [low, high). */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 val = (__int128) mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t col = n - r * (r + 1) / 2;   /* column within the row (0‑based) */

    if (col == 0)
        return (seq_t)1;                    /* C(r,0) - 0  */

    seq_t top   = binomial(r, col);        /* C(r, col)   */
    seq_t below = binomial(r, col - 1);    /* C(r, col-1) */

    return top - below;                    /* difference of consecutive binomial coefficients */
}
