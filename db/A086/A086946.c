/*
 * A086946  Numbers k such that R(k+6) = 2.
 * https://oeis.org/A086946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*(10^n-3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset (n == 0) return 0; the OEIS offset is 1. */
    if (n == 0)
        return (seq_t)0;

    seq_t ten_pow = 1;          /* ten_pow will hold 10^n */
    for (idx_t i = 0; i < n; ++i)
        ten_pow *= (seq_t)10;

    /* a(n) = 2*(10^n-3) = 2*ten_pow - 6 */
    seq_t result = ten_pow * (seq_t)2;
    result -= (seq_t)6;
    return result;
}
