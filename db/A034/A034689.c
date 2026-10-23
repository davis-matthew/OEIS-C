/*
 * A034689  a(n) = n-th sextic factorial number divided by 2.
 * https://oeis.org/A034689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = 2 * (3*n - 2) * a(n-1) for n >= 2. */
seq_t a(idx_t n)
{
    /* Start with the first term. */
    seq_t result = (seq_t)1;

    /* Compute succeeding terms multiplicatively. */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* factor = 2 * (3*i - 2) */
        seq_t three_i   = (seq_t)3 * i;
        three_i -= (seq_t)2;
        seq_t factor = (seq_t)2 * three_i;

        result *= factor;
    }

    return result;
}
