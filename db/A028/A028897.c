/*
 * A028897  If n = Sum c_i 10^i then a(n) = Sum c_i 2^i.
 * https://oeis.org/A028897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice, look
 * Formula:   a(n) = Σ c_i * 2^i where n = Σ c_i * 10^i (decimal digits)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow2   = 1;
    while (n > 0) {
        result += (seq_t)(n % 10) * pow2;
        n /= 10;
        pow2 <<= 1;
    }
    return result;
}
