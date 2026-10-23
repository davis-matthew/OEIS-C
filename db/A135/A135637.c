/*
 * A135637  Values of y in positive solutions to the Diophantine equation
 *          43x+7y+17z=400.
 * https://oeis.org/A135637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = y of the nth positive solution (x,y,z) to 43·x + 7·y + 17·z = 400,
   ordered by increasing x and, for equal x, by increasing y. */

/* The sequence is finite; there are 13 solutions.  For n > 13 the
   function returns 0. */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;           /* number of solutions found so far */

    for (int x = 1; ; ++x) {
        int64_t R = 400 - 43 * x;          /* remaining sum for 7y + 17z */
        if (R < 24) break;                 /* 7*1 + 17*1 = 24 is minimum residual */

        /* iterate possible y values; y is at most R/7 */
        int max_y = (int)(R / 7);
        for (int y = 1; y <= max_y; ++y) {
            int64_t rem = R - 7LL * y;      /* remaining for 17z */
            if (rem <= 0) continue;        /* z must be positive */
            if (rem % 17 != 0) continue;
            int64_t z = rem / 17;
            if (z < 1) continue;           /* z must be positive */

            ++count;
            if (count == n) return (seq_t)y;
        }
    }

    return 0;           /* n is beyond the number of solutions */
}
