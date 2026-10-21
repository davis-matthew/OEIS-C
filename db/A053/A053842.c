/*
 * A053842  (Sum of digits of n written in base 7) modulo 7.
 * https://oeis.org/A053842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (sum of digits of n written in base 7) modulo 7
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t m = n;
    while (m) {
        sum += (seq_t)(m % 7);
        m /= 7;
    }
    return sum % 7;
}
