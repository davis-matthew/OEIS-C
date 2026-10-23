/*
 * A199018  a(n) = (3*11^n - 1)/2.
 * https://oeis.org/A199018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + 5; a(0) = 1 derivable from a(n) = (3*11^n -
 *            1)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;          /* a(0) */
    idx_t i;

    for (i = 0; i < n; ++i) {
        val = val * 11 + 5;
    }

    return val;
}
