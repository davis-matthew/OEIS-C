/*
 * A213909  Sum of all even numbers in Collatz (3x+1) trajectory of n.
 * https://oeis.org/A213909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all even numbers in the Collatz (3x+1) trajectory
 *            of n, starting from n and terminating at 1. If n is odd, it is
 *            not counted; if n is 1 the sum is 0
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* current value in the trajectory */
    seq_t cur = (seq_t)n;
    seq_t sum = 0;

    while (cur != (seq_t)1) {
        if ((cur & (seq_t)1) == (seq_t)0) {
            sum += cur;
            cur = cur >> 1;              /* divide by 2 */
        } else {
            cur = (seq_t)3 * cur + (seq_t)1;
        }
    }
    return sum;
}
