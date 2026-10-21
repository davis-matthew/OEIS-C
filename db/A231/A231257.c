/*
 * A231257  Number of (n+1) X (2+1) 0..2 arrays with no element unequal to a
 *          strict majority of its horizontal and antidiagonal neighbors, with
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A231257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 29*a(n-2) + 36*a(n-3) - 16*a(n-4) for n≥5,
 *            a(1)=3, a(2)=15, a(3)=89, a(4)=547
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset, return 0 */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)89;
    if (n == 4) return (seq_t)547;

    /* compute using the recurrence */
    seq_t a1 = (seq_t)3;
    seq_t a2 = (seq_t)15;
    seq_t a3 = (seq_t)89;
    seq_t a4 = (seq_t)547;
    seq_t ai;

    for (idx_t i = 5; i <= n; ++i) {
        ai = (seq_t)10 * a4 - (seq_t)29 * a3 + (seq_t)36 * a2 - (seq_t)16 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return a4;
}
