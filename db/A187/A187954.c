/*
 * A187954  Triangle by rows, divisors of odd integers.
 * https://oeis.org/A187954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2k+1) if (2k+1) divides (2r+1), otherwise 0. Here r is
 *            the row index such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2, and k = n
 *            – r*(r+1)/2 is the column position within that row
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r using binary search on the triangular numbers. */
    idx_t low = 0, high = n;          /* high is always ≥ r */
    idx_t r = 0;                      /* will hold the result */

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 off = (__int128)mid * (mid + 1) / 2;
        if (off <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Position k within the found row. */
    __int128 offR = (__int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)offR;

    /* The odd numbers to test. (2k+1) and (2r+1). */
    seq_t odd = (seq_t)(((__int128)2 * k) + 1);
    seq_t m   = (seq_t)(((__int128)2 * r) + 1);

    return (m % odd == 0) ? odd : (seq_t)0;
}
