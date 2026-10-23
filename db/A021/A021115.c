/*
 * A021115  Decimal expansion of 1/111.
 * https://oeis.org/A021115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 9 if (n mod 3) == 2 else 0 (periodic sequence [0,0,9])
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    return (r == 2) ? (seq_t)9 : (seq_t)0;
}
