/*
 * A035013  One third of 9-factorial numbers.
 * https://oeis.org/A035013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (∏_{j=1..n} (9*j-6))/3 (OEIS A035013)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        prod = prod * ((seq_t)9 * i - 6);
    }
    return prod / 3;
}
