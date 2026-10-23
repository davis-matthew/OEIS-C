/*
 * A188707  Number of 3 X n binary arrays without the pattern 0 0 diagonally
 *          or vertically.
 * https://oeis.org/A188707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) with a(1)=5, a(2)=21
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two values directly */
    if (n == 1)
        return (seq_t)5;
    if (n == 2)
        return (seq_t)21;

    seq_t prev2 = (seq_t)5;   /* a(1) */
    seq_t prev1 = (seq_t)21;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur  = (seq_t)5 * prev1 - (seq_t)3 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
