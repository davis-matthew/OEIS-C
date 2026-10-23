/*
 * A123010  a(n) = 5*a(n-1) + a(n-2) - 5*a(n-3), for n>4, with a(1)=1, a(2)=0,
 *          a(3)=4, a(4)=16.
 * https://oeis.org/A123010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + a(n-2) - 5*a(n-3), initial values: a(1)=1,
 *            a(2)=0, a(3)=4, a(4)=16
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the known initial segment */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)16;

    /* variables hold a(n-3), a(n-2), a(n-1) for the current step */
    seq_t prev3 = (seq_t)0;   /* a(n-3) */
    seq_t prev2 = (seq_t)4;   /* a(n-2) */
    seq_t prev1 = (seq_t)16;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = 5 * prev1 + prev2 - 5 * prev3;   /* a(i) */

        /* shift the window forward */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }

    return prev1;
}
