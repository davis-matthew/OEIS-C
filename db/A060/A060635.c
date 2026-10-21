/*
 * A060635  a(n) is the number of 2 X 1 domino tilings of the set S in the
 *          plane R^2 consisting of the union of the following two rectangles:
 *          rectangle1: |x| <= n, |y| <= 1, rectangle2: |x| <= 1, |y| <= n.
 * https://oeis.org/A060635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   fib_fast_doubling
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2 * F(n)^2 * F(n+1)^2   (F(0)=0, F(1)=1)
 */

#include <stdint.h>
#include "../utils/fib_fast_doubling.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_fast_doubling(n, &fn, &fn1);   /* fn = F(n), fn1 = F(n+1) */

    seq_t prod   = fn * fn1;           /* F(n) * F(n+1)   */
    seq_t square = prod * prod;       /* (F(n)*F(n+1))^2*/
    return square * 2;                /* 2 * square     */
}
