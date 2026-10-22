/*
 * A006788  a(n) = floor(2^(n-1)/n).
 * https://oeis.org/A006788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2^(n-1)/n), computed by binary long division of
 *            2^(n-1) by n
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 71, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q = 0;
    uint64_t r = 0;
    idx_t i;
    for (i = 0; i < n; i++) {
        uint64_t bit = (i == 0) ? 1 : 0;
        r = 2 * r + bit;
        q = q * 2;
        if (r >= n) {
            r -= n;
            q = q + 1;
        }
    }
    return q;
}
