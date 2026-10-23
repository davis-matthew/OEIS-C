/*
 * A172123  Number of ways to place 2 nonattacking bishops on an n X n board.
 * https://oeis.org/A172123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(3*n^2 - n + 2)/6
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not part of the OEIS offset, but safe */
    seq_t nn = (seq_t)n;
    seq_t a_val = nn;
    seq_t b_val = nn - (seq_t)1;
    seq_t c_val = (seq_t)3 * nn * nn - nn + (seq_t)2;

    /* divide by 2 */
    if (a_val % (seq_t)2 == 0)
        a_val /= (seq_t)2;
    else
        b_val /= (seq_t)2;

    /* divide by 3 */
    if (a_val % (seq_t)3 == 0)
        a_val /= (seq_t)3;
    else if (b_val % (seq_t)3 == 0)
        b_val /= (seq_t)3;
    else
        c_val /= (seq_t)3;

    return a_val * b_val * c_val;
}
