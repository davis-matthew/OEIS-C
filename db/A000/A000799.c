/*
 * A000799  a(n) = floor(2^n / n).
 * https://oeis.org/A000799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2^n / n), computed by binary long division of 2^n
 *            by n
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 70, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q = 0;
    idx_t r = 1;
    idx_t i;
    if (r >= n) {
        q = 1;
        r -= n;
    }
    for (i = 0; i < n; i++) {
        r <<= 1;
        q <<= 1;
        if (r >= n) {
            q += 1;
            r -= n;
        }
    }
    return q;
}
