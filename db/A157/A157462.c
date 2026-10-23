/*
 * A157462  Denominator of Euler(n, 1/30).
 * https://oeis.org/A157462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 15^{ceil(n/2)} * 60^{floor(n/2)}
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        if (i & 1) {
            res *= (seq_t)15;
        } else {
            res *= (seq_t)60;
        }
    }
    return res;
}
