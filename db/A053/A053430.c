/*
 * A053430  a(n) = (6^(n+1) - (-5)^(n+1))/11.
 * https://oeis.org/A053430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6^(n+1) - (-5)^(n+1)) / 11 Equivalent recurrence:
 *            a(0)=1, a(1)=1, a(n) = a(n-1) + 30*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)1;    /* a(0) */
    seq_t a1 = (seq_t)1;    /* a(1) */
    seq_t a2 = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        a2 = a1 + (seq_t)30 * a0; /* a(i) = a(i-1) + 30*a(i-2) */
        a0 = a1;
        a1 = a2;
    }
    return a2;
}
