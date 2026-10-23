/*
 * A014297  a(n) = n! * C(n+2, 2) * 2^(n+1).
 * https://oeis.org/A014297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)! * 2^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n + 2; ++i) {
        result *= (seq_t)i;
    }
    for (idx_t i = 0; i < n; ++i) {
        result *= 2;
    }
    return result;
}
