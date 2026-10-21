/*
 * A233328  a(n) = (2*8^(n+1) - 9) / 7.
 * https://oeis.org/A233328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*8^(n+1) - 9) / 7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternate simple recurrence: a(0)=1, a(n)=8*a(n-1)+9 for n>0 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t val = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        val = val * (seq_t)8 + (seq_t)9;
    }
    return val;
}
