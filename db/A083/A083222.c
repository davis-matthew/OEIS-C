/*
 * A083222  a(n) = (4*5^n + (-5)^n)/5.
 * https://oeis.org/A083222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*5^n + (-5)^n)/5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow5 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow5 *= (seq_t)5;
    }

    /* For even n, a(n) = 5^n. For odd n, a(n) = 3*5^(n-1) */
    if ((n & 1U) == 0) {
        return pow5;
    } else {
        return (pow5 / (seq_t)5) * (seq_t)3;
    }
}
