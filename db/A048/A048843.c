/*
 * A048843  Least positive integer k for which 17^n divides k!.
 * https://oeis.org/A048843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer k such that 17^n divides k!
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: 17^0 = 1 divides k! for any k; the smallest positive k is 1. */
    if (n == 0)
        return (seq_t)1;

    /* Binary search for minimal k with v17(k!) >= n.
       v17(k!) = floor(k/17) + floor(k/17^2) + … */
    seq_t low = (seq_t)1;
    seq_t high = ((seq_t)n) * (seq_t)17 + (seq_t)16;   /* safe upper bound */

    while (low < high) {
        seq_t mid = (low + high) / (seq_t)2;

        /* Compute v17(mid!) */
        seq_t tmp = mid;
        seq_t exp = (seq_t)0;
        while (tmp) {
            tmp /= (seq_t)17;
            exp += tmp;
        }

        if (exp >= (seq_t)n) {
            high = mid;
        } else {
            low = mid + (seq_t)1;
        }
    }

    return low;
}
