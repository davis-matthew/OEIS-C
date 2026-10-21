/*
 * A201081  Number of -1..1 arrays of n elements with first and second
 *          differences also in -1..1.
 * https://oeis.org/A201081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) + a(n-4) with initial values
 *            a(1)=3, a(2)=7, a(3)=13, a(4)=25
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 33, u64 n <= 68, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset 1 */
    if (n == 0)
        return (seq_t)0;   /* not part of the OEIS data */

    /* Return the known first four terms directly */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)13;
    if (n == 4) return (seq_t)25;

    /* Use a sliding window of the last four computed values. */
    seq_t a1 = (seq_t)3;    /* a(n-4)  */
    seq_t a2 = (seq_t)7;    /* a(n-3)  */
    seq_t a3 = (seq_t)13;   /* a(n-2)  */
    seq_t a4 = (seq_t)25;   /* a(n-1)  */
    seq_t an;              /* a(n)    */

    for (idx_t i = 5; i <= n; ++i)
    {
        an = (2 * a4) - a3 + a2 + a1;   /* recurrence */
        /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
