/*
 * A056953  Denominators of continued fraction for alternating factorial.
 * https://oeis.org/A056953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + floor(n/2) * a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 37, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Previous two values of the sequence */
    seq_t a_prev2 = (seq_t)1; /* a(0) */
    seq_t a_prev1 = (seq_t)1; /* a(1) */
    seq_t a_cur   = a_prev1;  /* dummy initialization */

    /* Iteratively compute up to the requested index */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* floor(i/2) multiplied by a(i-2) */
        seq_t addend = ((seq_t)(i >> 1)) * a_prev2;
        a_cur = a_prev1 + addend;

        /* Shift the window for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_cur;
}
