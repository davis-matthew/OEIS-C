/*
 * A129801  Triangle read by rows in which row m (m>=0) gives the numbers
 *          2*m*n + 1 for n = 0, ..., m.
 * https://oeis.org/A129801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*m*p + 1 where m = smallest integer such that
 *            (m+1)*(m+2)/2 >= n (row index starting at 0) and p = n -
 *            (m*(m+1)/2) - 1 (position in that row, 0‑based) The function
 *            works for any idx_t n >= 1 and returns the value in seq_t
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int tri_ge(idx_t k, idx_t n)
{
    /* returns true if k*(k+1)/2 >= n, computed with 128‑bit to avoid overflow */
    return (__int128)k * (k + 1) / 2 >= n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but harmless */

    /* find smallest k such that k*(k+1)/2 >= n   (k = m+1) */
    idx_t low = 1, high = 1;
    while (!tri_ge(high, n))
        high <<= 1;                /* double until condition holds */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (tri_ge(mid, n))
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;                 /* k = m + 1 */
    idx_t m = k - 1;
    idx_t prev = (k - 1) * k / 2;  /* terms before this row */
    idx_t pos0 = n - prev - 1;     /* 0‑based position in row */

    /* compute value in seq_t */
    seq_t val = 2 * (seq_t)m * (seq_t)pos0 + 1;
    return val;
}
