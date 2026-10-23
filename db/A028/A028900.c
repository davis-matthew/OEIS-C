/*
 * A028900  Map n = Sum c_i 10^i to a(n) = Sum c_i 5^i.
 * https://oeis.org/A028900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Sum_i c_i * 5^i, where c_i are the decimal digits of n
 *            (least significant digit has i = 0)
 * Range:     u8 n <= 955, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow5   = 1;
    while (n > 0) {
        unsigned digit = n % 10;     /* current decimal digit */
        result += (seq_t)digit * pow5;
        pow5 *= 5;
        n /= 10;
    }
    return result;
}
