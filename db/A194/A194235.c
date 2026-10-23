/*
 * A194235  a(n) = [Sum_{k=1..n} (k/8)], where [ ]=floor, ( )=fractional part.
 * https://oeis.org/A194235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sum_{k=1..n} frac(k/8) )
 * Range:     u8 n <= 360, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q = (seq_t)(n / 8);
    seq_t r = (seq_t)(n % 8);
    seq_t temp = q * 28 + (r * (r + 1)) / 2;
    return temp / 8;
}
