/*
 * A199484  a(n) = (8*7^n+1)/3.
 * https://oeis.org/A199484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * a(n-1) - 2, a(0) = 3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)3;        /* a(0) = 3 */
    for (idx_t i = 1; i <= n; ++i) {
        val = (seq_t)7 * val - (seq_t)2;
    }
    return val;
}
