/*
 * A089944  Square array, read by antidiagonals, where the n-th row is the
 *          n-th binomial transform of the natural numbers, with T(0,k) =
 *          (k+1) for k>=0.
 * https://oeis.org/A089944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = antidiagonal read of T(n,k) where * T(n,0) = 1, * T(n,k)
 *            = (k + n + 1) * (n + 1)^(k - 1) for k >= 1
 * Range:     u8 n <= 28, u16 n <= 67, u32 n <= 156, u64 n <= 412,
 *            u128 n <= 1136, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned __int128 value. */
static idx_t isqrt128(unsigned __int128 x)
{
    idx_t lo = 0;
    idx_t hi = ((idx_t)1 << 34);   /* > sqrt(8·(2^64-1)+1) */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* 1. Find diagonal d: d(d+1)/2 <= n < (d+1)(d+2)/2 */
    unsigned __int128 t = (unsigned __int128)8 * n + 1;   /* 8*n+1 */
    idx_t s = isqrt128(t);
    idx_t d = (s - 1) / 2;                /* diagonal index */

    /* 2. Position i inside the diagonal, 0 <= i <= d */
    unsigned __int128 prev = (unsigned __int128)d * (d + 1) / 2;
    idx_t i = n - (idx_t)prev;

    /* 3. Map to array coordinates: row = i, col = d - i
     *    (antidiagonal order: (0,d), (1,d-1), ..., (d,0)) */
    idx_t row = i;
    idx_t col = d - i;

    /* 4. Compute T(row, col) */
    if (col == 0)
        return (seq_t)1;           /* T(row,0)=1 */

    seq_t base = (seq_t)(row + 1);      /* base = (row+1) */
    seq_t pow_val = (seq_t)1;
    for (idx_t e = 1; e < col; ++e)
        pow_val *= base;               /* (row+1)^(col-1) */

    seq_t coeff = (seq_t)(row + col + 1);   /* (k+n+1) */
    return coeff * pow_val;                /* full T(n,k) */
}
