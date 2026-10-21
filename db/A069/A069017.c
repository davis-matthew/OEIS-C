/*
 * A069017  Triangular numbers of the form k^2 + k + 1.
 * https://oeis.org/A069017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-2) - a(n-4) - 10 initial values: a(1)=1, a(2)=3,
 *            a(3)=21, a(4)=91
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle out‑of‑range requests arbitrarily */
    if (n < 1) return 0;

    /* Direct return for the first four terms */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)21;
    if (n == 4) return (seq_t)91;

    /* Keep only the last four values needed by the recurrence.
       buf[0] = a(k-4), buf[1] = a(k-3), buf[2] = a(k-2), buf[3] = a(k-1)   */
    seq_t buf[4];
    buf[0] = (seq_t)1;   /* a(1) */
    buf[1] = (seq_t)3;   /* a(2) */
    buf[2] = (seq_t)21;  /* a(3) */
    buf[3] = (seq_t)91;  /* a(4) */

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t new_term = (seq_t)34 * buf[2] - buf[0] - (seq_t)10;

        /* shift the window forward */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = new_term;
    }

    return buf[3];
}
