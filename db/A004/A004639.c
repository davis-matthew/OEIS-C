/*
 * A004639  Cubes written in base 9.
 * https://oeis.org/A004639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007095(n^3), where A007095 converts to base 9
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1188, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;
    seq_t result = 0;
    seq_t multiplier = 1;
    
    while (cube > 0) {
        seq_t digit = cube % 9;
        result = result + digit * multiplier;
        multiplier = multiplier * 10;
        cube = cube / 9;
    }
    
    return result;
}
