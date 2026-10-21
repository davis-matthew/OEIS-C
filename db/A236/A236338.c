/*
 * A236338  Number of iterations of A235600 to reach 1 when starting with n,
 *          or -1 if 1 is never reached. (A235600(x) = x/sum_of_digits(x) if
 *          this is an integer, otherwise A235600(x) = x.)
 * https://oeis.org/A236338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 68, i16 n <= 68, i32 n <= 68, i64 n <= 68, i128 n <= 68,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of iterations of A235600 needed to reach 1, or -1 if 1 is never reached.
   A235600(x) = x / digit_sum(x) if divisible, otherwise x itself.  */
/* This implementation follows the OEIS definition exactly, handling the special case
   where division by 1 would lead to an infinite loop (e.g., 10). */
#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* current value in the iteration */
    seq_t count = 0;             /* number of successful divisions done */

    if (x == 1) return 0;        /* already at the target */

    while (1) {
        seq_t s = digit_sum(x);  /* sum of decimal digits of x */

        /* If the sum is 1 and x != 1, division would leave x unchanged,
           so the process never reaches 1. */
        if (s == 1) return -1;

        if (x % s != 0)          /* not divisible, fixed point other than 1 */
            return -1;

        x /= s;                  /* successful division */
        count++;

        if (x == 1)              /* reached the goal */
            return count;
    }
}
