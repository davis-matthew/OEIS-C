/*
 * A133140  a(0) = 2, a(n) = 2^n + 2 for n>=1.
 * https://oeis.org/A133140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2; a(1) = 4; a(n) = 2*a(n-1) - 2 (for n >= 2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)4;
    seq_t val = (seq_t)4;
    for (idx_t i = 2; i <= n; ++i) {
        val = val * (seq_t)2 - (seq_t)2;
    }
    return val;
}
