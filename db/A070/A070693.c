/*
 * A070693  a(n) = n^7 mod 11.
 * https://oeis.org/A070693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 11
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Reduce n modulo 11 to keep intermediate values small */
    idx_t nm = n % 11;
    seq_t x = (seq_t)nm;
    seq_t mod = (seq_t)11;
    seq_t result = 1;
    for (int i = 0; i < 7; ++i) {
        result = (result * x) % mod;
    }
    return result;
}
