/*
 * A053208  Row sums of A053207.
 * https://oeis.org/A053208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=3, a(n+1)=2*a(n)+4
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    /* compute iteratively: a(1)=3, then a(k)=2*a(k-1)+4 for k>=2 */
    seq_t value = (seq_t)3;
    for (idx_t i = 2; i <= n; ++i)
        value = value * (seq_t)2 + (seq_t)4;
    return value;
}
