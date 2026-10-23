/*
 * A183223  Complement of the 11-gonal numbers.
 * https://oeis.org/A183223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(1/2+(2n/9+2/9)^(1/2))
 * Range:     u8 n <= 14, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(x)) for non-negative x. */
static seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = x;
    while (lo <= hi) {
        seq_t mid = lo + (hi - lo) / 2;
        seq_t mid2 = mid * mid;
        if (mid2 == x)
            return mid;
        if (mid2 < x) {
            lo = mid + 1;
        } else {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    seq_t d = (seq_t)n * 2 + 2;          /* d = 2n + 2 */
    seq_t s = floor_sqrt(d);             /* s = floor(sqrt(d))   */

    /* m = nearest integer to sqrt(d)/3 */
    seq_t q = s / 3;
    seq_t r = s % 3;
    seq_t m = q;
    if (r == 2) {
        m = q + 1;
    } else if (r == 1) {
        /* check whether fractional part >= 0.5:
           sqrt(d) >= s + 0.5   <=>   (2s+1)^2 <= 4*d
        */
        seq_t left = (2 * s + 1);
        left = left * left;                 /* (2s+1)^2 */
        seq_t right = 4 * d;                /* 4*d    */
        if (left <= right)
            m = q + 1;
    }
    return (seq_t)n + m;
}
