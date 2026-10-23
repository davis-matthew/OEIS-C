/*
 * A158856  Triangle T(n, k) = coefficients of p(n, x), where p(n, x) = (1 -
 *          x^(2+floor((n-1)/2)))*(1 + (-1)^floor(n/2)*x^(1+floor(n/2))), read
 *          by rows.
 * https://oeis.org/A158856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = coefficient of x^k in (∑_{j=0}^{1+⌊(r-1)/2⌋} x^j) ·
 *            (∑_{i=0}^{⌊r/2⌋} (-x)^i), where r satisfies r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2 and k = n - r(r+1)/2
 * Range:     i8 n <= 1325, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the unique r such that r(r+1)/2 <= n < (r+1)(r+2)/2 (rows of the triangle). */
static inline idx_t find_row(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 0, high = 1;
    /* Increase high until it is large enough. */
    while (( (__int128)high * (high + 1) / 2 ) <= n)
        high <<= 1;
    /* Binary search between low and high. */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (( (__int128)mid * (mid + 1) / 2 ) <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Special case for the first entry. */
    if (n == 0) return (seq_t)1;

    /* Determine which row the index lies in. */
    idx_t r = find_row(n);

    /* Index within the row. */
    idx_t k = n - (idx_t)(((__int128)r * (r + 1)) / 2);

    /* Bounds for the i–sum (coefficients from the second factor). */
    idx_t max_i = r / 2;
    idx_t max_j = (r == 0) ? 0 : (1 + (r - 1) / 2);

    /* Effective range for i: 0 ≤ i ≤ max_i and 0 ≤ k-i ≤ max_j. */
    idx_t i_start = (k > max_j) ? (k - max_j) : 0;
    idx_t i_end   = (k < max_i) ? k : max_i;

    if (i_start > i_end) return (seq_t)0;

    seq_t res = 0;
    for (idx_t i = i_start; i <= i_end; ++i) {
        res += ( (i & 1) ? (seq_t)(-1) : (seq_t)1 );
    }
    return res;
}
