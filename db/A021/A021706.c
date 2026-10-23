/*
 * A021706  Decimal expansion of 1/702.
 * https://oeis.org/A021706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0) = 0, a(1) = 0; for n ≥ 2, a(n) = [1, 4, 2, 4, 5, 0][(n-2)
 *            mod 6]
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021706: Decimal expansion of 1/702 */

seq_t a(idx_t n)
{
    static const seq_t table[6] = { 1, 4, 2, 4, 5, 0 };
    if (n < 2)
        return 0;
    return table[(n - 2) % 6];
}
