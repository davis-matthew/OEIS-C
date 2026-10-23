/*
 * A123380  Values x of the solutions (x,y) of the Diophantine equation
 *          5*(X-Y)^4 - 8*X*Y = 0 with X >= Y.
 * https://oeis.org/A123380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1480*a(n-1) - 54798*a(n-2) + 1480*a(n-3) - a(n-4) for n
 *            >= 4 a(0) = 0 a(1) = 120 a(2) = 164616 a(3) = 237056040
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle first four terms explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)120;
    if (n == 2) return (seq_t)164616;
    if (n == 3) return (seq_t)237056040;

    /* Only the last four values are needed */
    seq_t a0 = (seq_t)0;               /* a(n-4)   */
    seq_t a1 = (seq_t)120;             /* a(n-3)   */
    seq_t a2 = (seq_t)164616;          /* a(n-2)   */
    seq_t a3 = (seq_t)237056040;       /* a(n-1)   */
    seq_t an;                          /* a(n)     */

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)1480 * a3
            - (seq_t)54798 * a2
            + (seq_t)1480 * a1
            - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
