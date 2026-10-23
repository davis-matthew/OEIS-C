/*
 * A131087  Triangle read by rows: T(n,k) = 2*binomial(n,k) - (1 +
 *          (-1)^(n-k))/2 (0 <= k <= n).
 * https://oeis.org/A131087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(n,k) = 2*binomial(n,k) - (1 +
 *            (-1)^(n-k))/2, r and k are determined from the flat index n
 *            (0‑based) as the row and column in the triangular array
 * Range:     u8 n <= 39, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

static inline idx_t triangular(idx_t r)
/*  r*(r+1)/2  (no overflow for idx_t within typical limits) */
{
    return r * (r + 1) / 2;
}

static inline idx_t find_row(idx_t pos)
/* Find the row r such that triangular(r) <= pos < triangular(r+1). */
{
    idx_t low = 0, high = 1;
    /* Grow high until triangular(high) > pos or high reaches half the max value. */
    while ((unsigned __int128)high * (high + 1) <= (unsigned __int128)pos * 2 && high < ((idx_t)-1) / 2)
        high <<= 1;
    /* Binary search on [low, high) */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= pos)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Handle the very first element explicitly. */
    if (n == 0)
        return 1;

    /* Determine the row r and column k from the flat index n. */
    idx_t r = find_row(n);
    idx_t k = n - triangular(r);

    /* Compute binomial coefficient. */
    seq_t bin = binom(r, k);

    /* Compute the parity term:
        parity = ((r - k) % 2 == 0) ? 1 : 0   (i.e. (1 + (-1)^(r-k))/2)
    */
    idx_t parity = ((r - k) & 1) ? 0 : 1;

    /* T(r,k) = 2*bin - parity */
    return bin * (seq_t)2 - (seq_t)parity;
}
