/*
 * A159955  Period 9: repeat [0, 1, 2, 1, 2, 0, 2, 0, 1].
 * https://oeis.org/A159955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + floor(n/3)) mod 3
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n mod 3 and floor(n/3) mod 3 to avoid overflow */
    idx_t r1 = n % 3;
    idx_t r2 = (n / 3) % 3;
    idx_t r = (r1 + r2) % 3;
    return (seq_t)r;
}
