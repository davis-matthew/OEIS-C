/*
 * A177057  Decimal expansion of 7/6.
 * https://oeis.org/A177057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(1)=1, a(2)=1, a(n)=6 for n ≥ 3 (digits of 7/6)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;
    return (seq_t)6;
}
