/*
 * A021031  Decimal expansion of 1/27.
 * https://oeis.org/A021031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 0 if n%3==0; 3 if n%3==1; 7 if n%3==2
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    if (r == 0) return 0;
    if (r == 1) return 3;
    return 7;
}
