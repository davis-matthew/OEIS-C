/*
 * A062119  a(n) = n! * (n-1).
 * https://oeis.org/A062119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * (n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    if (n > 1) {
        for (idx_t i = 2; i <= n; ++i) {
            res *= (seq_t)i;
        }
    }
    res *= (seq_t)(n - 1);
    return res;
}
