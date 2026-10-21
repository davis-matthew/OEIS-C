/*
 * A226234  Triangle defined by T(n,k) = binomial(n^2, k^2), for n>=0, k=0..n,
 *          as read by rows.
 * https://oeis.org/A226234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r^2, k^2) where r is the row index such that
 *            S(r-1) ≤ n < S(r) with S(t) = (t+1)(t+2)/2, and k = n - S(r-1)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 24, u64 n <= 49, u128 n <= 84,
 *            big any n
 * Helpers:   binomial_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function internally computes binomial(n, k) safely for both
   fixed‑width integers and arbitrary‑precision types. */

#include <stdint.h>
#include "../utils/binomial_square.h"

seq_t a(idx_t n)
{
    /* Binary search for the row r: smallest r with S(r) > n */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)(mid + 1) * (mid + 2) / 2;
        if (val > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* offset within that row */
    idx_t before = r * (r + 1) / 2;
    idx_t k = n - before;

    return binomial_square(r, k);
}
