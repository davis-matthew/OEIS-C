/*
 * A104881  Triangle T(n,k) = Sum_{j=0..k} (n-k)^(k-j), read by rows.
 * https://oeis.org/A104881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = sum_{m=0..c} (r-c)^m, where r = floor((sqrt(8*n+1)-1)/2)
 *            is the row, c = n - r*(r+1)/2 is the column, and T(r,c) =
 *            Σ_{j=0..c} (r-c)^(c-j) = Σ_{m=0..c} (r-c)^m
 * Range:     u8 n <= 39, u16 n <= 84, u32 n <= 200, u64 n <= 482,
 *            u128 n <= 1207, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t row_of_index(idx_t n)
{
    idx_t low = 0, high = n;
    /* Binary search for the largest r with r*(r+1)/2 <= n */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t row = row_of_index(n);
    idx_t prev = (idx_t)((unsigned __int128)row * (row + 1) / 2);
    idx_t col = n - prev;

    seq_t base = (seq_t)(row - col);   /* p = r - c */
    seq_t result = 1;                  /* term for power 0 */
    seq_t pow = 1;                     /* current p^m, starts at p^0=1 */

    /* Add powers p^1 ... p^col */
    for (idx_t i = 0; i < col; ++i) {
        pow *= base;
        result += pow;
    }
    return result;
}
