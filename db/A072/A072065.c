/*
 * A072065  Define a "piece" to consist of 3 mutually touching pennies welded
 *          together to form a triangle; sequence gives side lengths of
 *          triangles that can be made from such pieces.
 * https://oeis.org/A072065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) (n >= 6) Initial values: a(1)=0,
 *            a(2)=2, a(3)=9, a(4)=11, a(5)=12 The sequence is strictly
 *            increasing after the first five terms. This implementation
 *            simply evaluates the recurrence iteratively
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly. */
    switch (n)
    {
    case 1: return (seq_t)0;
    case 2: return (seq_t)2;
    case 3: return (seq_t)9;
    case 4: return (seq_t)11;
    case 5: return (seq_t)12;
    default:
        /* Initial five values to start the recurrence. */
        seq_t a1 = (seq_t)0;   /* a(n-5) at the start */
        seq_t a2 = (seq_t)2;   /* a(n-4)  */
        seq_t a3 = (seq_t)9;   /* a(n-3)  */
        seq_t a4 = (seq_t)11;  /* a(n-2)  */
        seq_t a5 = (seq_t)12;  /* a(n-1)  */
        seq_t next;
        idx_t i;

        for (i = 6; i <= n; ++i)
        {
            /* a(i) = a5 + a2 - a1  (a(n-1)+a(n-4)-a(n-5)) */
            next = a5 + a2 - a1;

            /* shift the window for the next iteration */
            a1 = a2;
            a2 = a3;
            a3 = a4;
            a4 = a5;
            a5 = next;
        }
        return a5;  /* a(n) after the loop */
    }
}
