/*
 * A192955  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5), with
 *            a(0)=0, a(1)=1, a(2)=2, a(3)=7, a(4)=18
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial terms */
    static const seq_t init[5] = {0, 1, 2, 7, 18};

    if (n < 5)
        return init[n];

    /* We keep the last five values in a sliding window. */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];

    for (idx_t i = 5; i <= n; ++i) {
        /* Apply the recurrence */
        seq_t a5 = 4 * a4 - 5 * a3 + a2 + 2 * a1 - a0;

        /* Shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
    }

    return a4; /* a(n) */
}
