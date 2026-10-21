/*
 * A056487  a(n) = 5^(n/2) for n even, a(n) = 3*5^((n-1)/2) for n odd.
 * https://oeis.org/A056487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n/2) for even n; a(n) = 3*5^((n-1)/2) for odd n
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* exponent: floor(n/2) works for both even and odd */
    idx_t exp = n / 2;
    /* start value depends on parity of n */
    seq_t res = (n & 1) ? 3 : 1;
    /* multiply by 5 exp times */
    for (idx_t i = 0; i < exp; ++i)
        res *= 5;
    return res;
}
