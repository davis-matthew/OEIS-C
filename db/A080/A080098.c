/*
 * A080098  Triangle T(n,k) = n OR k, 0 <= k <= n, bitwise logical OR, read by
 *          rows.
 * https://oeis.org/A080098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 9894, u16 n <= 9894, u32 n <= 9894, u64 n <= 9894,
 *            u128 n <= 9894, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:
   For index n, find the row r of the triangle T(n,k)=n|k
   such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.
   Then k = n - r(r+1)/2  and the value is r | k.
   The search for r is performed by binary search using 128‑bit
   arithmetic to avoid overflow.  The computation is O(log n). */

seq_t a(idx_t n)
{
    /* Find the largest r with r*(r+1)/2 ≤ n */
    idx_t low = 0, high = n, r = 0;
    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n)
        {
            r = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Position within the row */
    idx_t prefix = (__uint128_t)r * (r + 1) / 2;
    idx_t k = n - prefix;

    /* Return the bitwise OR of r and k */
    return (seq_t)r | (seq_t)k;
}
