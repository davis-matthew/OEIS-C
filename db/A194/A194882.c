/*
 * A194882  Write n = C(i,4)+C(j,3)+C(k,2)+C(l,1) with i>j>k>l>=0; sequence
 *          gives i values.
 * https://oeis.org/A194882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest integer i with binom(i,4) ≤ n
 * Range:     u8 n <= 5, u16 n <= 118, u32 n <= 118, u64 n <= 118,
 *            u128 n <= 118, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find an upper bound high such that binom(high,4) > n */
    idx_t low  = 0;
    idx_t high = 1;
    seq_t sn = (seq_t)n;

    while (binom(high, 4) <= sn) {
        if (high > ((idx_t)-1) >> 1) {   /* avoid overflow of high */
            high = (idx_t)-1;
            break;
        }
        high <<= 1;
    }

    /* Binary search for the largest i with binom(i,4) ≤ n */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (binom(mid, 4) <= sn)
            low = mid;
        else
            high = mid;
    }

    return (seq_t)low;
}
