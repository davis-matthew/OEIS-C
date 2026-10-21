/*
 * A073424  Triangle read by rows: T(m,n) = parity of 0^n + 0^m, n = 0,1,2,3
 *          ..., m = 0,1,2,3, ... n.
 * https://oeis.org/A073424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 8864, u32 n <= 8864, u64 n <= 8864,
 *            u128 n <= 8864, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A073424:  a(n) = 1 iff n is a triangular number > 0 (i.e. 8*n+1 is a perfect square).
    Otherwise a(n) = 0. */

#include <stdbool.h>

static seq_t
isqrt_seq(seq_t x)
{
    /* Returns the floor of the square root of x using only integer operations. */
    if (x < 2) return x;          /* sqrt(0)=0, sqrt(1)=1 */

    seq_t low = 1, high = x, mid;
    while (low <= high) {
        mid = low + ((high - low) >> 1);

        /* mid^2 <= x  iff  mid <= x / mid   (avoids overflow) */
        if (mid > x / mid) {
            high = mid - 1;        /* mid^2 > x */
        } else {
            /* mid^2 <= x: check for equality */
            if (mid == x / mid && x % mid == 0) {
                return mid;        /* exact square root */
            }
            low = mid + 1;         /* mid^2 < x */
        }
    }
    return high;                    /* floor sqrt */
}

seq_t
a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;            /* a(0) = 0 by definition */
    }

    /* Compute d = 8*n + 1  */
    seq_t d = n * (seq_t)8 + (seq_t)1;

    seq_t s = isqrt_seq(d);

    /* Return 1 if d is a perfect square, else 0 */
    return (s == d / s && d % s == 0) ? (seq_t)1 : (seq_t)0;
}
