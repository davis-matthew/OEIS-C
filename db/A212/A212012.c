/*
 * A212012  Triangle read by rows in which row n lists the number of states of
 *          the subshells of the n-th shell of the nuclear shell model ordered
 *          by energy level in increasing order.
 * https://oeis.org/A212012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2 * ( (m*(m+1))/2 - n + 1 ), where m =
 *            ceil((√(8n+1)-1)/2) is the row containing n
 * Range:     u8 n <= 8128, u16 n <= 9406, u32 n <= 9406, u64 n <= 9406,
 *            u128 n <= 9406, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row number m such that T(m-1) < n <= T(m),
       where T(m) = m*(m+1)/2.  Use binary search for O(log m). */
    idx_t low = 1, high = 1;

    /* Ensure high is large enough. */
    while ( ( (__uint128_t)high * (high + 1) ) / 2 < n )
        high <<= 1;   /* double high */

    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ( (__uint128_t)mid * (mid + 1) ) / 2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Compute the value for this row. */
    __uint128_t Tm = (__uint128_t)m * (m + 1) / 2;
    seq_t diff = (seq_t)(Tm - n + 1);   /* positive and <= m */
    return diff * 2;
}
