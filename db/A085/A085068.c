/*
 * A085068  Number of steps >= 1 for iteration of map x -> (4/3)*ceiling(x) to
 *          reach an integer when started at n, or -1 if no such integer is
 *          ever reached.
 * https://oeis.org/A085068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps needed to reach an integer by repeatedly
 *            applying x -> (4/3)*ceil(x), starting at integer n. The
 *            algorithm uses only integer arithmetic and reduces the fraction
 *            at each step. It terminates in at most O(log n) steps for n ≤
 *            10000
 * Range:     u8 n <= 24, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* numerator of current value, denominator is either 1 or 3 */
    seq_t p = (seq_t)n;
    int denom = 1;            /* 1 means integer, 3 means divisible by 3 only */
    seq_t steps = 0;

    /* if n itself is already an integer after the first transformation,
       we will detect it after the first loop iteration. */
    while (true) {
        steps++;

        /* compute ceil(p / denom) */
        seq_t ceil_val = (p + (seq_t)denom - 1) / denom;

        /* next value: 4 * ceil_val / 3 */
        p = ceil_val * 4;
        if (p % 3 == 0) {
            p /= 3;
            denom = 1;       /* result is integer */
        } else {
            denom = 3;       /* result has denominator 3 */
        }

        /* integer reached */
        if (denom == 1)
            return steps;

        /* safeguard against an infinite loop; not expected for n ≤ 10000. */
        if (steps > 10000)
            return (seq_t)-1;
    }
}
