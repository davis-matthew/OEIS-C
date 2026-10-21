/*
 * A092200  Expansion of (1+2x)/((1-x)(1-x^3)).
 * https://oeis.org/A092200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n+2 if n mod 3 == 1, otherwise a(n) = n+1
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n % 3) == 1)
        return (seq_t)n + 2;
    return (seq_t)n + 1;
}
