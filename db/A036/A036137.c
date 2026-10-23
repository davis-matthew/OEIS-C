/*
 * A036137  a(n) = 5^n mod 97.
 * https://oeis.org/A036137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 97
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const seq_t modulus = 97;
    const seq_t base_init = 5;
    seq_t result = 1;
    seq_t base = base_init;
    idx_t exp = n % 96;          /* 5^(96) ≡ 1 (mod 97) */
    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % modulus;
        base = (base * base) % modulus;
        exp >>= 1;
    }
    return result;
}
