/*
 * A141317  a(n) = A000244(n) - A010684(n).
 * https://oeis.org/A141317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2 + (-1)^n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t i = 0; i < n; ++i) {
        val *= (seq_t)3;
    }

    /* Add (+1) for even n, subtract (-1) for odd n */
    if ((n & 1) == 0) {
        val += (seq_t)1;
    } else {
        val -= (seq_t)1;
    }

    /* Subtract the constant term 2 */
    val -= (seq_t)2;

    return val;
}
