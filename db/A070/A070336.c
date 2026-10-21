/*
 * A070336  a(n) = 2^n mod 25.
 * https://oeis.org/A070336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 25
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Since 2 and 25 are coprime, Euler's theorem gives 2^20 ≡ 1 (mod 25),
       so we reduce the exponent modulo 20. */
    idx_t exp = n % 20;
    seq_t value = 1;
    for (idx_t i = 0; i < exp; ++i) {
        value = (value * 2) % 25;
    }
    return value;
}
