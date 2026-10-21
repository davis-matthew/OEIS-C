/*
 * A071006  Number of positive solutions to the Diophantine equation
 *          x^2+y^3=n^2 with y <= x.
 * https://oeis.org/A071006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 77, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of positive integer solutions (x, y)
   to the Diophantine equation x^2 + y^3 = n^2 with y <= x.
   The routine iterates over all possible y, checks whether
   n^2 - y^3 is a perfect square, and counts the solutions. */
#include <stdint.h>

/* Integer square root (floor) for any seq_t; uses only seq_t arithmetic. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t low = 0;
    seq_t high = v;
    seq_t mid;
    while (low <= high) {
        mid = (low + high) >> 1;          /* (low+high)/2 using bit shift */
        seq_t mid2 = mid * mid;
        if (mid2 == v) return mid;
        if (mid2 < v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Compute n^2 in seq_t */
    seq_t n_val = (seq_t)n;
    seq_t n2 = n_val * n_val;

    seq_t count = 0;

    for (idx_t y = 1; ; ++y) {
        seq_t y_val = (seq_t)y;
        seq_t y2 = y_val * y_val;
        seq_t y3 = y2 * y_val;

        /* Stop when y^3 is not less than n^2 */
        if (y3 >= n2) break;

        seq_t t = n2 - y3;
        if (t == 0) continue;           /* x would be zero, not positive */

        seq_t s = isqrt_seq(t);
        if (s * s == t && y_val <= s)
            ++count;
    }

    return count;
}
