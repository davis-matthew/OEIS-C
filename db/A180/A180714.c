/*
 * A180714  Sum of the x- and y-coordinates of a point moving in a clockwise
 *          spiral.
 * https://oeis.org/A180714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum of the x‑ and y‑coordinates of the point after * n
 *            unit steps in the clockwise square spiral (up, right, down,
 *            left) * with step lengths 1,1,2,2,3,3,…
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Find the largest m such that m*(m+1) <= n.  That is the number
     * of complete pairs of steps already finished.
     */
    uint64_t m = (uint64_t)((sqrtl((long double)1 +
                                      (long double)4 * (long double)n) - 1) / 2);

    /* R = sum_{odd i<=m} i  -  sum_{even i<=m} i  = 2*odd_sum - total_sum */
    seq_t odd_cnt = (seq_t)((m + 1) / 2);           /* number of odd i */
    seq_t odd_sum = odd_cnt * odd_cnt;              /* sum of odd i */
    seq_t total_sum = (seq_t)m * ((seq_t)m + 1) / 2;
    seq_t R = odd_sum * 2 - total_sum;             /* x = y = R after full pairs */

    uint64_t steps_done = m * (m + 1);             /* steps after complete pairs */
    uint64_t rem = n - steps_done;                 /* remaining unit steps */

    seq_t x = R, y = R;
    uint64_t seg_len = m + 1;                      /* length of the current pair */

    if (rem == 0) {
        return x + y;                              /* sum 2*R */
    }

    if (seg_len & 1) {                             /* odd segment length: up then right */
        if (rem <= seg_len) {                      /* still in the 'up' segment */
            y += (seq_t)rem;
        } else {                                   /* completed 'up', now 'right' segment */
            y += (seq_t)seg_len;
            x += (seq_t)(rem - seg_len);
        }
    } else {                                       /* even segment length: down then left */
        if (rem <= seg_len) {                      /* still in the 'down' segment */
            y -= (seq_t)rem;
        } else {                                   /* completed 'down', now 'left' segment */
            y -= (seq_t)seg_len;
            x -= (seq_t)(rem - seg_len);
        }
    }

    return x + y;
}
