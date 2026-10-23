/*
 * A011944  a(n) = 14*a(n-1) - a(n-2) with a(0) = 0, a(1) = 2.
 * https://oeis.org/A011944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 14*a(n-1) - a(n-2) with a(0)=0, a(1)=2 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;

    seq_t a0 = 0;
    seq_t a1 = 2;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t a2 = (seq_t)14 * a1 - a0;
        a0 = a1;
        a1 = a2;
    }

    return a1;
}
