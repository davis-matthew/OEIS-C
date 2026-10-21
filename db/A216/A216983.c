/*
 * A216983  The integers sieved by 7, 5, 3, and 2.
 * https://oeis.org/A216983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 if n mod 7 == 1; 5 if n mod 5 == 1; 3 if n mod 3 == 1;
 *            otherwise 1 for even n, 2 for odd n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 7 == 1) return (seq_t)7;
    if (n % 5 == 1) return (seq_t)5;
    if (n % 3 == 1) return (seq_t)3;
    return ((n % 2) == 0 ? (seq_t)1 : (seq_t)2);
}
