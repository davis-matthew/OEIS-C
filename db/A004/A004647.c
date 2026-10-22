/*
 * A004647  Powers of 2 written in base 8.
 * https://oeis.org/A004647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = c * 10^(n // 3), where c = 1, 2, 4 for n ≡ 0, 1, 2 (mod
 *            3)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t coeff;
    
    switch (n % 3) {
        case 0: coeff = 1; break;
        case 1: coeff = 2; break;
        case 2: coeff = 4; break;
    }
    
    idx_t power = n / 3;
    seq_t result = coeff;
    for (idx_t i = 0; i < power; i++) {
        result *= 10;
    }
    
    return result;
}
