/*
 * A165792  a(0)=1, a(n) = n*(a(n-1)+2).
 * https://oeis.org/A165792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=n*(a(n-1)+2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = 1;               /* a(0) = 1 */
    for (idx_t i = 1; i <= n; ++i)
        value = ((seq_t)i) * (value + 2);
    return value;
}
