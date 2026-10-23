/*
 * A185738  Rectangular array T(n,k) = 2^n + k - 2, by antidiagonals.
 * https://oeis.org/A185738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * Let t = minimal integer with t(t-1)/2 >= n (triangular index)
 *            * Let i = n - (t-1)(t-2)/2 (row index within diagonal) * Let j =
 *            t - i (column index within diagonal) * Then a(n) = 2^i + j - 2 *
 *            * The function computes the value by first determining t with a
 *            binary * search, then finding i and j,...
 * Range:     u8 n <= 43, u16 n <= 151, u32 n <= 559, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    idx_t low = 1, high = 2;

    /* Find an upper bound for t */
    while ((__int128)high * (high - 1) / 2 < n)
        high <<= 1;

    /* Binary search for the smallest t with t(t-1)/2 >= n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((__int128)mid * (mid - 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t t = low;

    /* Number of terms before diagonal t */
    idx_t prev_sum = ((__int128)(t - 1) * (t - 2)) / 2;
    idx_t pos = n - prev_sum;          /* 1‑based position in diagonal */
    idx_t i = pos;                     /* row index inside diagonal   */
    idx_t j = t - pos;                 /* column index               */

    seq_t val = pow2(i);               /* 2^i */
    val += (seq_t)j;                   /* + j */
    val -= 2;                          /* - 2   */
    return val;
}
