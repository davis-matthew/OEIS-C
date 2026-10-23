/*
 * A198766  a(n) = (7*5^n - 1)/2.
 * https://oeis.org/A198766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*5^n - 1)/2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 3; recurrence: a(n) = 5*a(n-1) + 2 for n > 0 */

seq_t a(idx_t n)
{
    /* start with the initial term a(0) */
    seq_t value = (seq_t)3;

    /* for n == 0 the loop body is skipped */
    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        /* use only integer arithmetic; seq_t supports operator* and + */
        value = value * (seq_t)5 + (seq_t)2;
    }

    return value;
}
