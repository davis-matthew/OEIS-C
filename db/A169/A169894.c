/*
 * A169894  Table of carryless sums i + j, i>=0, j>=0, read by antidiagonals.
 * https://oeis.org/A169894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, tabl
 * Formula:   a(n) = carryless sum of i and j, where i+j=d, d =
 *            ⌊(√(8n+1)-1)/2⌋, i = n - d(d+1)/2, j = d - i
 * Range:     u8 n <= 9945, u16 n <= 9945, u32 n <= 9945, u64 n <= 9945,
 *            u128 n <= 9945, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n)
{
    /* Find diagonal d:   d*(d+1)/2 ≤ n < (d+1)*(d+2)/2 */
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n) {
        hi <<= 1;               /* double until above n */
    }
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    idx_t d = lo;
    idx_t prefix = (unsigned __int128)d * (d + 1) / 2;
    idx_t offset = n - prefix;
    idx_t i = offset;
    idx_t j = d - offset;

    /* Compute carryless sum of i and j */
    seq_t val   = 0;
    seq_t power = 1;
    while (i > 0 || j > 0) {
        uint32_t digit = (uint32_t)((i % 10 + j % 10) % 10);
        val += (seq_t)digit * power;
        power *= (seq_t)10;
        i /= 10;
        j /= 10;
    }
    return val;
}
