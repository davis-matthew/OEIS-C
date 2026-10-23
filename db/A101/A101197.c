/*
 * A101197  Lengths of successive words (starting with a) under the
 *          substitution: {a -> aab, b -> aac, c -> b}.
 * https://oeis.org/A101197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 2*a(n-3), a(0) = 1, a(1) = 3, a(2)
 *            = 9
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial three terms explicitly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;
    if (n == 2)
        return (seq_t)9;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)3;   /* a(1) */
    seq_t a2 = (seq_t)9;   /* a(2) */
    seq_t a_next;

    /* Compute subsequent terms up to a(n) */
    for (idx_t i = 3; i <= n; ++i)
    {
        a_next = 2 * a2 + 3 * a1 - 2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a_next;
    }

    return a2;
}
