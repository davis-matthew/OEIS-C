/*
 * A093850  Triangle T(n,k) = 10^(n-1) -1 + k*floor(9*10^(n-1)/(n+1)), with 1
 *          <= r <= n, read by rows.
 * https://oeis.org/A093850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, base
 * Formula:   T(n,k) = 10^(n‑1) - 1 + k*floor(9*10^(n‑1)/(n+1)) Sequence a(n)
 *            is the terms read by rows of the triangle T(n,k)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 49, u64 n <= 191, u128 n <= 751,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th term.
       r is the smallest integer with r*(r+1)/2 >= n. */
    idx_t low = 1, high = 1;
    while (((unsigned __int128)high * (high + 1)) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (((unsigned __int128)mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column k (1‑based) within the found row. */
    idx_t prev_terms = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - prev_terms;               /* 1‑based index inside row r */

    /* Compute pow10 = 10^(r-1) as a seq_t. */
    seq_t pow10 = 1;
    for (idx_t i = 1; i < r; ++i) pow10 *= 10;

    /* base = 10^(r-1) - 1. */
    seq_t base = pow10 - 1;

    /* step = floor(9*10^(r-1)/(r+1)) computed without overflow. */
    seq_t q    = pow10 / (r + 1);           /* floor division */
    seq_t rem  = pow10 % (r + 1);
    seq_t step = q * 9 + (rem * 9) / (r + 1);

    /* T(r,k) = base + k*step. */
    return base + (seq_t)k * step;
}
