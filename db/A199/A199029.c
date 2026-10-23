/*
 * A199029  a(n) = 9*11^n - 1.
 * https://oeis.org/A199029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*11^n - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow11 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow11 *= (seq_t)11;
    }
    pow11 *= (seq_t)9;
    pow11 -= (seq_t)1;
    return pow11;
}
