/*
 * A212832  Decimal expansion of 5/24.
 * https://oeis.org/A212832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(0)=2, a(1)=0, a(2)=8, a(n)=3 for all n≥3
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)8;
    return (seq_t)3;
}
