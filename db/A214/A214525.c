/*
 * A214525  a(n) = 7*a(n-1) - 23*a(n-2) + 49*a(n-3) - 49*a(n-4) with a(0)=0,
 *          a(1)=1, a(2)=7, a(3)=19.
 * https://oeis.org/A214525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 23*a(n-2) + 49*a(n-3) - 49*a(n-4) * Initial
 *            values: a(0)=0, a(1)=1, a(2)=7, a(3)=19
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)19;

    /* Keep the last four values of the sequence. */
    seq_t a0 = (seq_t)0;   /* a(i-4)   */
    seq_t a1 = (seq_t)1;   /* a(i-3)   */
    seq_t a2 = (seq_t)7;   /* a(i-2)   */
    seq_t a3 = (seq_t)19;  /* a(i-1)   */
    seq_t next;

    /* Iterate from 4 up to n, updating the four previous values each time. */
    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)(7 * a3) - (seq_t)(23 * a2) + (seq_t)(49 * a1) - (seq_t)(49 * a0);
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
