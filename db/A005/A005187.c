/*
 * A005187  a(n) = a(floor(n/2)) + n; also denominators in expansion of
 *          1/sqrt(1-x) are 2^a(n); also 2n - number of 1's in binary
 *          expansion of 2n.
 * https://oeis.org/A005187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k >= 0} floor(n/2^k)
 * Range:     u8 n <= 128, u16 n <= 9651, u32 n <= 9651, u64 n <= 9651,
 *            u128 n <= 9651, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power = (seq_t)n;
    while (power > 0) {
        result += power;
        power >>= 1;
    }
    return result;
}
