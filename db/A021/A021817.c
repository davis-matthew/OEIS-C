/*
 * A021817  Decimal expansion of 1/813.
 * https://oeis.org/A021817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ((n-1) mod 5) mod 4
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t values[5] = {0, 0, 1, 2, 3};
    return values[n % 5];
}
