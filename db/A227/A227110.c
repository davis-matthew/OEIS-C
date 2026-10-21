/*
 * A227110  Nonnegative solutions of the Pell equation x^2 - 89*y^2 = +1.
 *          Solutions x = a(n).
 * https://oeis.org/A227110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*500001*a(n-1) - a(n-2); a(0)=1, a(1)=500001
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)500001;

    seq_t prevprev = 1;
    seq_t prev     = 500001;
    seq_t curr     = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = (2*500001)*a(i-1) - a(i-2)  */
        curr = prev * 1000002 - prevprev;
        prevprev = prev;
        prev = curr;
    }
    return prev;
}
