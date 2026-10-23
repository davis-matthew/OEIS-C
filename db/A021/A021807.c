/*
 * A021807  Decimal expansion of 1/803.
 * https://oeis.org/A021807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 803) mod 10 (decimal digit of 1/803)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[8] = {0, 0, 1, 2, 4, 5, 3, 3};
    return period[n & 7];
}
