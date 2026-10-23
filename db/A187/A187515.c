/*
 * A187515  Number of 10-step S, E, and NW-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3603*n^2 - 28504*n + 54377 for n>8 Initial values:
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=398, a(5)=3868, a(6)=13796,
 *            a(7)=31548, a(8)=56952, a(9)=89684 Recurrence:
 *            a(n)=3*a(n-1)-3*a(n-2)+a(n-3) for n>11
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Known values for 1 ≤ n ≤ 8 */
    static const seq_t small[] = {
        (seq_t)0,          /* index 0 unused */
        (seq_t)0,          /* a(1) */
        (seq_t)0,          /* a(2) */
        (seq_t)0,          /* a(3) */
        (seq_t)398,        /* a(4) */
        (seq_t)3868,       /* a(5) */
        (seq_t)13796,      /* a(6) */
        (seq_t)31548,      /* a(7) */
        (seq_t)56952       /* a(8) */
    };

    if (n <= 8)
        return small[(idx_t)n];

    seq_t nn = (seq_t)n;
    seq_t result = (seq_t)3603 * nn * nn;   /* 3603*n^2 */
    result -= (seq_t)28504 * nn;            /* -28504*n */
    result += (seq_t)54377;                 /* +54377 */
    return result;
}
