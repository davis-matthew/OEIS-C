/*
 * A004637  Cubes written in base 7.
 * https://oeis.org/A004637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007093(n^3), where A007093 converts to base 7
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 563, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute n^3
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;
    
    // Convert n^3 to base 7 representation as a decimal number
    seq_t result = 0;
    seq_t power = 1;
    
    while (cube > 0) {
        seq_t digit = cube % 7;
        result = result + digit * power;
        power = power * 10;
        cube = cube / 7;
    }
    
    return result;
}
