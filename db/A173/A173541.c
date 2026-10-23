/*
 * A173541  Triangle read by rows: T(n,k)=k if k is a proper non-divisor of n,
 *          otherwise T(n,k)=0 (1<=k<=n).
 * https://oeis.org/A173541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = k if k is a proper non‑divisor of r, otherwise 0, where r
 *            and k are obtained by reading the triangle T(r,k) row by row.
 *            The triangle is defined by T(r,k) = k if k∤r , otherwise 0 (1 ≤
 *            k ≤ r)
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any idx_t and seq_t types supplied by the build, */
/* using only integer arithmetic.  It is O(log r) per call with a fast   */
/* integer search for the row number. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)           /* the offset is 1; n=0 is outside the sequence */
        return 0;

    /* Find the row r such that r*(r+1)/2 >= n and (r-1)*r/2 < n. */
    idx_t low = 1, high = 1;
    /* Increase high until it is large enough */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;

    /* Binary search on [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Position k in the row */
    idx_t k = n - (unsigned __int128)r * (r - 1) / 2;

    /* Return k if k does not divide r, else 0 */
    if (k == 0)   /* safety; k should never be 0 because 1 ≤ k ≤ r */
        return 0;
    return (r % k == 0) ? 0 : (seq_t)k;
}
