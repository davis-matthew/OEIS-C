/*
 * A110749  Triangle read by rows with the n-th row containing the first n
 *          multiples of n with digits reversed.
 * https://oeis.org/A110749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, look, nonn, tabl
 * Formula:   a(n) is the reverse of r·k, where r is the smallest integer with
 *            r(r+1)/2 ≥ n (the row containing the n‑th term) and k = n –
 *            r(r–1)/2 is the position within that row
 * Range:     u8 n <= 74, u16 n <= 5249, u32 n <= 9708, u64 n <= 9708,
 *            u128 n <= 9708, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev.h"

seq_t a(idx_t n)
{
    if (n == 0)       /* n is never 0 in OEIS, but keep function defined */
        return 0;

    /* Binary search for the row r such that tri(r‑1) < n ≤ tri(r), where
       tri(r) = r(r+1)/2.  Use 128‑bit arithmetic to avoid overflow. */
    idx_t low  = 1;
    idx_t high = n;
    idx_t r = n;           /* initial value; will be overwritten */
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
        {
            r = mid;
            if (mid == 0)   /* safety for 0­-based loops */
                break;
            high = mid - 1;
        }
        else
            low = mid + 1;
    }

    /* Number of terms before row r */
    unsigned __int128 prev128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t prev = (idx_t)prev128;

    /* Position within the row */
    idx_t k = n - prev;

    /* Value is the product r·k, reversed */
    seq_t term = (seq_t)r * (seq_t)k;
    return rev(term);
}
