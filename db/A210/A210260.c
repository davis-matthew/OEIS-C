/*
 * A210260  Number of (n+1) X 2 0..3 arrays containing all values 0..3 with
 *          every 2 X 2 subblock having three distinct values, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A210260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 18*a(n-2) - 64*a(n-3) - 24*a(n-4), a(1)=0,
 *            a(2)=29, a(3)=400, a(4)=4298
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term */
    if (n == 0)
        return 0;   /* behaviour below the offset is irrelevant */

    /* base cases */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)29;
    if (n == 3) return (seq_t)400;
    if (n == 4) return (seq_t)4298;

    /* iteratively compute the recurrence up to n */
    seq_t a1 = (seq_t)0;        /* a(n-4) */
    seq_t a2 = (seq_t)29;       /* a(n-3) */
    seq_t a3 = (seq_t)400;      /* a(n-2) */
    seq_t a4 = (seq_t)4298;     /* a(n-1) */
    seq_t val;

    for (idx_t i = 5; i <= n; ++i) {
        val = (seq_t)12 * a4
              - (seq_t)18 * a3
              - (seq_t)64 * a2
              - (seq_t)24 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = val;
    }

    return a4;
}
