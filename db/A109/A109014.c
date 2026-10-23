/*
 * A109014  a(n) = gcd(n,11).
 * https://oeis.org/A109014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 + 10 * [11 divides n]
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 11 if 11 | n, otherwise 1.        */

seq_t a(idx_t n)
{
    return (n % 11ULL == 0) ? 11 : 1;
}
