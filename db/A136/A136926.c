/*
 * A136926  Numbers k such that k and k^2 use only the digits 0, 2, 7 and 9.
 * https://oeis.org/A136926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 0; a(n) = 27 * 10^(n-2) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return 0;
    }
    /* compute 10^(n-2) */
    seq_t res = 1;
    idx_t e = n - 2;
    for (idx_t i = 0; i < e; ++i) {
        res *= 10;
    }
    /* multiply by 27 */
    return res * 27;
}
