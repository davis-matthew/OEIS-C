/*
 * A155038  Triangle read by rows: T(n,k) is the number of compositions of n
 *          with first part k.
 * https://oeis.org/A155038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the smallest integer such that r(r+1)/2 >= n (row
 *            number). Let pos = n - r(r-1)/2 (position in that row, 1 ≤ pos ≤
 *            r). Then a(n) = 1 if pos == r, otherwise a(n) = 2^(r - pos - 1).
 *            This counts the number of compositions of an integer n with
 *            first part k when the triangle is read by rows
 * Range:     u8 n <= 45, u16 n <= 153, u32 n <= 561, u64 n <= 2145,
 *            u128 n <= 5050, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* n = 0 is not part of the offset */

    /* Find the row r using binary search on r(r+1)/2 >= n */
    idx_t low = 1;
    idx_t high = n;  /* high is safe because the row number never exceeds n */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute position within the row */
    unsigned __int128 prev_sum = (unsigned __int128)(r - 1) * r / 2;
    idx_t pos = n - prev_sum;  /* 1‑based index within the row */

    if (pos == r)
        return (seq_t)1;

    /* Return 2^(r - pos - 1) */
    return pow2(r - pos - 1);
}
