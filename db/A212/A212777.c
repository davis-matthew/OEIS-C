/*
 * A212777  Half the number of 0..3 arrays of length n+2 with second
 *          differences nonzero.
 * https://oeis.org/A212777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) + 3*a(n-3) - 4*a(n-4) - 4*a(n-5)
 *            Initial values: a(1)=28, a(2)=99, a(3)=350, a(4)=1238, a(5)=4379
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Return the known initial terms directly. */
    if (n <= 5)
    {
        switch (n)
        {
            case 1: return (seq_t)28;
            case 2: return (seq_t)99;
            case 3: return (seq_t)350;
            case 4: return (seq_t)1238;
            case 5: return (seq_t)4379;
        }
    }

    /* Compute the sequence iteratively up to n. */
    seq_t t[5];
    t[0] = (seq_t)28;   /* a(1)   */
    t[1] = (seq_t)99;   /* a(2)   */
    t[2] = (seq_t)350;  /* a(3)   */
    t[3] = (seq_t)1238; /* a(4)   */
    t[4] = (seq_t)4379; /* a(5)   */

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t next = (seq_t)(2 * t[4] + 5 * t[3] + 3 * t[2] - 4 * t[1] - 4 * t[0]);

        /* Shift the window of the last five terms. */
        t[0] = t[1];
        t[1] = t[2];
        t[2] = t[3];
        t[3] = t[4];
        t[4] = next;
    }

    return t[4];
}
