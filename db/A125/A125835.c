/*
 * A125835  Numbers whose base-8 or octal representation is 22222222.......2.
 * https://oeis.org/A125835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(8^(n-1)-1)/7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; for safety handle n == 0 */
    if (n == 0) return 0;

    /* compute 8^(n-1) as a seq_t */
    seq_t pow8 = 1;
    for (idx_t i = 1; i < n; ++i) {
        pow8 *= 8;
    }

    /* a(n) = 2 * (pow8 - 1) / 7   (division is exact for all n >= 1) */
    seq_t tmp = pow8 - 1;
    tmp /= 7;
    tmp *= 2;
    return tmp;
}
