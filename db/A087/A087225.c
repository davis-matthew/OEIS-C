/*
 * A087225  Position of the largest peak value in Collatz (3x+1) trajectory.
 * https://oeis.org/A087225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the largest value reached while iterating the
 *            Collatz (3x+1) trajectory starting at n. The first element of
 *            the trajectory has position 1. The function is purely iterative
 *            and uses only integer arithmetic
 * Range:     u8 n <= 26, u16 n <= 6826, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = (seq_t)n;        /* current Collatz term */
    idx_t pos   = 1;               /* current position in the trajectory */
    seq_t peak  = value;           /* highest value seen so far */
    idx_t peakpos = 1;             /* position of the peak */

    /* Iterate until the sequence reaches 1 */
    while (value != (seq_t)1)
    {
        if ((value & (seq_t)1) == (seq_t)0) /* even */
            value = value / (seq_t)2;
        else                               /* odd */
            value = value * (seq_t)3 + (seq_t)1;

        pos++;                              /* advance position */

        if (value > peak)                   /* new peak found */
        {
            peak   = value;
            peakpos = pos;
        }
    }
    return (seq_t)peakpos;                 /* return position of the peak */
}
