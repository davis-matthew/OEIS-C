/*
 * A206190  Number of (n+1) X 2 0..3 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to two.
 * https://oeis.org/A206190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 7*a(n-2) + 4*a(n-3) + 2*a(n-4) Initial values:
 *            a(1) = 124, a(2) = 1012, a(3) = 8304, a(4) = 68180
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset; behaviour for n < 1 is undefined, but return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Base cases */
    seq_t a1 = (seq_t)124;   /* a(1) */
    seq_t a2 = (seq_t)1012;  /* a(2) */
    seq_t a3 = (seq_t)8304;  /* a(3) */
    seq_t a4 = (seq_t)68180; /* a(4) */

    switch (n)
    {
        case 1: return a1;
        case 2: return a2;
        case 3: return a3;
        case 4: return a4;
        default: break;
    }

    /* Iteratively compute up to a(n) using the recurrence. */
    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = (((seq_t)9 * a4) + ((seq_t)4 * a2) + ((seq_t)2 * a1))
                        - ((seq_t)7 * a3);
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
