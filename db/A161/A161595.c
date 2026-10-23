/*
 * A161595  The list of the A values in the common solutions to the 2
 *          equations 15*k+1=A^2, 19*k+1=B^2.
 * https://oeis.org/A161595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - a(n-2); with a(1)=1, a(2)=16 Recurrence
 *            relation of A161595
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)16;

    /* Iterate the recurrence up to n */
    seq_t a_prev = (seq_t)1;   /* a(1) */
    seq_t a_curr = (seq_t)16;  /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t a_next = (seq_t)17 * a_curr - a_prev;
        a_prev = a_curr;
        a_curr = a_next;
    }
    return a_curr;
}
