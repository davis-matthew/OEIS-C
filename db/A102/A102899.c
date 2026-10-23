/*
 * A102899  a(n) = ceiling(n/3)^2 - floor(n/3)^2.
 * https://oeis.org/A102899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n%3==0 else 2*floor(n/3)+1
 * Range:     u8 n <= 384, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n % 3) == 0) {
        return (seq_t)0;
    } else {
        seq_t k = (seq_t)(n / 3);
        return k * 2 + 1;
    }
}
