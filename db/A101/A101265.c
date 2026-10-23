/*
 * A101265  a(1) = 1, a(2) = 2, a(3) = 6; a(n) = 5*a(n-1) - 5*a(n-2) + a(n-3)
 *          for n > 3.
 * https://oeis.org/A101265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) + a(n-3); initial values a(1)=1,
 *            a(2)=2, a(3)=6
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the tiny indices directly */
    if (n == 0)   /* undefined by the OEIS offset; return 0 safely */
        return 0;

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;
    if (n == 3)
        return (seq_t)6;

    /* iterate using only the three most recent terms */
    seq_t a_n3 = (seq_t)6;   /* a(i-1) for i=4 starts at a(3)   */
    seq_t a_n2 = (seq_t)2;   /* a(i-2) for i=4 starts at a(2)   */
    seq_t a_n1 = (seq_t)1;   /* a(i-3) for i=4 starts at a(1)   */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(i) = 5*a(i-1) - 5*a(i-2) + a(i-3) */
        seq_t next = (seq_t)5 * a_n3 - (seq_t)5 * a_n2 + a_n1;

        /* shift the window for the next iteration */
        a_n1 = a_n2;
        a_n2 = a_n3;
        a_n3 = next;
    }

    return a_n3;   /* a(n) after the loop */
}
