/*
 * A015265  Gaussian binomial coefficient [ n,2 ] for q = -13.
 * https://oeis.org/A015265
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 157*a(n-1) + 2041*a(n-2) - 2197*a(n-3); initial values:
 *            a(2) = 1, a(3) = 157, a(4) = 26690
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  For n < 2 the values are undefined,
       so we return zero. */
    if (n < 2) return 0;

    /* Handle the initial terms directly. */
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)157;
    if (n == 4) return (seq_t)26690;

    /* Use a running recurrence with three variables to avoid large buffers.
       a1 = a(n-3), a2 = a(n-2), a3 = a(n-1). */
    seq_t a1 = (seq_t)1;          /* a(2)  */
    seq_t a2 = (seq_t)157;        /* a(3)  */
    seq_t a3 = (seq_t)26690;      /* a(4)  */

    for (idx_t i = 5; i <= n; ++i)
    {
        /* Compute a(i) = 157*a3 + 2041*a2 - 2197*a1 */
        seq_t next = (seq_t)157 * a3
                    + (seq_t)2041 * a2
                    - (seq_t)2197 * a1;

        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
