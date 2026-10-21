/*
 * A216125  a(n) = 5^n mod 1000.
 * https://oeis.org/A216125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 1000
 * Range:     u8 n <= 3, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t base   = 5;
    idx_t m = n;
    while (m > 0) {
        if (m & 1)
            result = (result * base) % 1000;
        base = (base * base) % 1000;
        m >>= 1;
    }
    return result;
}
