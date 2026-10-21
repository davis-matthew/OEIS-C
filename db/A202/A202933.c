/*
 * A202933  Number of (n+3) X 5 binary arrays with consecutive windows of four
 *          bits considered as a binary number nondecreasing in every row and
 *          column.
 * https://oeis.org/A202933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) with initial values a(1)=83521, a(2)=113856, a(3)=153874,
 *            a(4)=206145, a(5)=273683, a(6)=359970
 * Range:     u8 none, u16 none, u32 n <= 102, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values (1‑based index).  */
    static const seq_t init[6] =
    {
        (seq_t)83521,
        (seq_t)113856,
        (seq_t)153874,
        (seq_t)206145,
        (seq_t)273683,
        (seq_t)359970
    };

    if (n <= 6)
        return init[n - 1];

    /* Working variables hold a(n-6), a(n-5), … , a(n-1).  */
    seq_t a1 = init[0]; /* a(n-6) */
    seq_t a2 = init[1]; /* a(n-5) */
    seq_t a3 = init[2]; /* a(n-4) */
    seq_t a4 = init[3]; /* a(n-3) */
    seq_t a5 = init[4]; /* a(n-2) */
    seq_t a6 = init[5]; /* a(n-1) */

    for (idx_t i = 7; i <= n; ++i)
    {
        seq_t next = 6 * a6
                   - 15 * a5
                   + 20 * a4
                   - 15 * a3
                   + 6 * a2
                   - a1;

        /* Shift the window: drop a1, shift others left, insert next.  */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6;
}
