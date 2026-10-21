/*
 * A235895  Number of (n+1) X (1+1) 0..2 arrays with the minimum plus the
 *          maximum equal to the lower median plus the upper median in every 2
 *          X 2 subblock.
 * https://oeis.org/A235895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 10*a(n-2) - 8*a(n-3) with a(1) = 33, a(2) =
 *            125, a(3) = 473
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset 1, behaviour for n<1 irrelevant */

    /* handle the three initial terms */
    if (n == 1) return (seq_t)33;
    if (n == 2) return (seq_t)125;
    if (n == 3) return (seq_t)473;

    /* iterate the linear recurrence; only three previous values are needed */
    seq_t a1 = (seq_t)33;   /* a(1)   */
    seq_t a2 = (seq_t)125;  /* a(2)   */
    seq_t a3 = (seq_t)473;  /* a(3)   */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)7 * a3;
        ai -= (seq_t)10 * a2;
        ai -= (seq_t)8  * a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
