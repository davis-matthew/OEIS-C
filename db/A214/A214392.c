/*
 * A214392  If n mod 4 = 0 then a(n) = n/4, otherwise a(n) = n.
 * https://oeis.org/A214392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n/4 if n mod 4 == 0 else n
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n % 4 == 0) ? ((seq_t)n) / 4 : (seq_t)n;
}
