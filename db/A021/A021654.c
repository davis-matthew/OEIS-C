/*
 * A021654  Decimal expansion of 1/650.
 * https://oeis.org/A021654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of decimal expansion of 1/650 The expansion is
 *            0.001538461538..., having two leading zeros followed by a
 *            repeating period [1,5,3,8,4,6]
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    static const seq_t cycle[6] = { 1, 5, 3, 8, 4, 6 };
    return cycle[(n - 2) % 6];
}
