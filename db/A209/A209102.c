/*
 * A209102  Number of 3 X n 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A209102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) - 110*a(n-2) + 180*a(n-3) - 60*a(n-4) for n >
 *            5, with a(1) = 5, a(2) = 82, a(3) = 1326, a(4) = 20928, a(5) =
 *            329064. The function computes the nth term by iterating the
 *            recurrence; no dynamic allocation is required, so it works
 *            safely for any order of calls
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, but not used directly */

seq_t a(idx_t n)
{
    /* Base terms for n = 1 … 5 */
    static const seq_t base[5] = {
        (seq_t)5,
        (seq_t)82,
        (seq_t)1326,
        (seq_t)20928,
        (seq_t)329064
    };

    /* Return immediately for the first five terms */
    if (n <= 5)
        return base[n - 1];

    /* Initialise the last five values */
    seq_t a1 = base[0];   /* a(1)  */
    seq_t a2 = base[1];   /* a(2)  */
    seq_t a3 = base[2];   /* a(3)  */
    seq_t a4 = base[3];   /* a(4)  */
    seq_t a5 = base[4];   /* a(5)  */

    /* Compute successive terms up to a(n) */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 22 * a5 - 110 * a4 + 180 * a3 - 60 * a2;
        /* Shift the values for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;   /* a(n) */
}
