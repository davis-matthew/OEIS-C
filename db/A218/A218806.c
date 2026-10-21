/*
 * A218806  Hilltop maps: number of n X 4 binary arrays indicating the
 *          locations of corresponding elements not exceeded by any horizontal
 *          or antidiagonal neighbor in a random 0..3 n X 4 array.
 * https://oeis.org/A218806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*(a(n-1)+a(n-2)+a(n-3)) +
 *            9*(a(n-4)+a(n-5)+a(n-6)+a(n-7)) for n >= 8; initial values:
 *            a(1)=15, a(2)=249, a(3)=3969, a(4)=63499, a(5)=1015887,
 *            a(6)=16252713, a(7)=260019585
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        abort(); /* sequence offset starts at 1 */

    /* Handle the first seven terms directly */
    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)249;
    if (n == 3) return (seq_t)3969;
    if (n == 4) return (seq_t)63499;
    if (n == 5) return (seq_t)1015887;
    if (n == 6) return (seq_t)16252713;
    if (n == 7) return (seq_t)260019585;

    /* Sliding window of the last seven terms: prev[0] = a(n-7), ..., prev[6] = a(n-1) */
    seq_t prev[7];
    prev[0] = (seq_t)15;          /* a1 */
    prev[1] = (seq_t)249;         /* a2 */
    prev[2] = (seq_t)3969;        /* a3 */
    prev[3] = (seq_t)63499;       /* a4 */
    prev[4] = (seq_t)1015887;     /* a5 */
    prev[5] = (seq_t)16252713;    /* a6 */
    prev[6] = (seq_t)260019585;   /* a7 */

    for (idx_t i = 8; i <= n; ++i)
    {
        seq_t sum15 = prev[4] + prev[5] + prev[6];              /* a(n-3)+a(n-2)+a(n-1) */
        seq_t sum9  = prev[0] + prev[1] + prev[2] + prev[3];   /* a(n-7)+a(n-6)+a(n-5)+a(n-4) */
        seq_t next = (seq_t)15 * sum15 + (seq_t)9 * sum9;

        /* Shift left: drop prev[0], move others left, insert new term at the end */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = prev[6];
        prev[6] = next;
    }

    /* After loop, prev[6] holds a(n) */
    return prev[6];
}
