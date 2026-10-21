/*
 * A081344  Natural numbers in square maze arrangement, read by antidiagonals.
 * https://oeis.org/A081344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (i-1)^2 + i + (i-j)*(-1)^(i-1) if i >= j, a(n) = (j-1)^2
 *            + j - (j-i)*(-1)^(j-1) if i < j, where i = n - t*(t+1)/2, j =
 *            (t*t + 3*t + 4)/2 - n, t = floor((-1 + sqrt(8*n-7))/2)
 * Range:     u8 n <= 135, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 128‑bit unsigned integer */
static unsigned __int128
isqrt128(unsigned __int128 x)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1;   /* sqrt fits in 64 bits */
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid <= x / mid)   /* mid*mid <= x avoiding overflow */
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* n is 1-based; the definition is undefined for n < 1. */
    if (n == 0)
        return 0;

    /* Compute t = floor((-1 + sqrt(8*n-7))/2) */
    unsigned __int128 xn = (unsigned __int128)n * 8u - 7u;      /* 8*n-7 */
    unsigned __int128 sqrt_d = isqrt128(xn);
    idx_t t = (idx_t)((sqrt_d - 1u) >> 1);

    /* Compute i = n - t*(t+1)/2 */
    unsigned __int128 tt = (unsigned __int128)t * (t + 1u);
    idx_t i = n - (idx_t)(tt / 2u);

    /* Compute j = (t*t + 3*t + 4)/2 - n */
    unsigned __int128 jj = ((unsigned __int128)t * t + 3u * t + 4u) / 2u;
    idx_t j = (idx_t)jj - n;

    /* The formula uses the transposed indices: row = j, col = i */
    idx_t row = j;
    idx_t col = i;

    if (row >= col) {
        seq_t diff = (seq_t)(row - col);
        seq_t base = ((seq_t)(row - 1)) * (row - 1) + (seq_t)row;
        if ((row - 1) & 1u)
            return base - diff;    /* (-1)^(row-1) = -1 when row even */
        else
            return base + diff;    /* (+1) when row odd */
    } else {
        seq_t diff = (seq_t)(col - row);
        seq_t base = ((seq_t)(col - 1)) * (col - 1) + (seq_t)col;
        if ((col - 1) & 1u)        /* (-1)^(col-1) = -1 when col even */
            return base + diff;    /* subtraction of a negative -> addition */
        else
            return base - diff;    /* subtraction of a positive -> subtraction */
    }
}
