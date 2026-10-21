/*
 * A051125  Table T(n,k) = max{n,k} read by antidiagonals (n >= 1, k >= 1).
 * https://oeis.org/A051125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = max(i,j), where (i,j) is the position of the n-th element
 *            * in the table T(i,j)=max(i,j) read by antidiagonals (starting
 *            with (1,1)). * The antidiagonal with sum s=i+j contains the
 *            elements * (1,s-1),(2,s-2),..., (s-1,1)
 * Range:     u8 n <= 8000, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* find the antidiagonal sum s such that
     * (s-1)*s/2 >= n  and  (s-2)*(s-1)/2 < n
     */
    idx_t low = 2;
    idx_t high = 2;
    /* increase high until it covers n */
    while ((unsigned __int128)(high - 1) * high / 2 < n) {
        high <<= 1;
        if (high < low) high = low; /* overflow guard */
    }

    /* binary search for minimal s */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 cum = (unsigned __int128)(mid - 1) * mid / 2;
        if (cum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    /* compute offset within diagonal s (0-indexed) */
    unsigned __int128 prev_cum = (unsigned __int128)(s - 2) * (s - 1) / 2;
    idx_t offset = (idx_t)(n - prev_cum - 1); /* offset in [0, s-2] */

    /* coordinates */
    idx_t i = offset + 1;          /* row */
    idx_t j = s - i;               /* column */

    /* return the maximum as seq_t */
    seq_t ret = (seq_t)((i > j) ? i : j);
    return ret;
}
