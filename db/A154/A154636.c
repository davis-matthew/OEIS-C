/*
 * A154636  a(n) is the ratio of the sum of the bends of the circles that are
 *          drawn in the n-th generation of Apollonian packing to the sum of
 *          the bends of the circles in the initial configuration of 3
 *          circles.
 * https://oeis.org/A154636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 3*a(n-2), with a(0)=1, a(1)=2, a(2)=18
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)18;

    seq_t a0 = (seq_t)1;      /* a(n-2) */
    seq_t a1 = (seq_t)2;      /* a(n-1) */
    seq_t an  = (seq_t)0;     /* a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        if (i == 2)
        {
            an = (seq_t)18;   /* a(2) */
        }
        else
        {
            an = (seq_t)8 * a1 - (seq_t)3 * a0;
        }
        a0 = a1;
        a1 = an;
    }
    return an;
}
