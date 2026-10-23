/*
 * A194227  [sum{(2k/7) : 1<=k<=n}], where [ ]=floor, ( )=fractional part.
 * https://oeis.org/A194227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( sum_{k=1}^n frac(2k/7) ) which equals 3 * (n / 7)
 *            + offset[n % 7] with offset = {0, 0, 0, 1, 1, 2, 3}
 * Range:     u8 n <= 597, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t offsets[7] = {0, 0, 0, 1, 1, 2, 3};
    idx_t r = n % 7;               // remainder 0..6
    seq_t q = (seq_t)(n / 7);      // quotient n div 7
    return q * 3 + offsets[r];
}
