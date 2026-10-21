/*
 * A097432  Integer part of the hypotenuse of right triangles with consecutive
 *          integer legs.
 * https://oeis.org/A097432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(n^2 + (n+1)^2))
 * Range:     u8 n <= 10, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 2n^2 + 2n + 1 as a seq_t */
    seq_t sn = (seq_t)n;
    seq_t val = sn * (sn + 1);   /* n*(n+1) */
    val = val * 2 + 1;           /* 2n^2 + 2n + 1 */

    /* Integer square root of val by binary search
       using only allowed integer operations */
    seq_t low = 0;
    seq_t high = val;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        if (mid == 0) {           /* avoid division by zero */
            low = mid + 1;
            continue;
        }
        seq_t div = val / mid;    /* integer division */
        if (mid <= div) {
            low = mid + 1;
        } else {
            /* mid*mid > val */
            if (mid == 0) break; /* safety */
            high = mid - 1;
        }
    }
    return high;   /* floor(sqrt(val)) */
}
