/*
 * A033049  Sums of distinct powers of 13.
 * https://oeis.org/A033049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{i} bit_i(n) * 13^i (bit_i is the ith binary digit of
 *            n)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power  = 1;          /* 13^0 */
    while (n) {
        if (n & 1UL) {
            result += power;
        }
        power *= (seq_t)13;
        n >>= 1;
    }
    return result;
}
