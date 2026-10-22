/*
 * A004635  Cubes written in base 5.
 * https://oeis.org/A004635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007091(n^3)
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 207, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;
    seq_t result = 0;
    seq_t power = 1;
    
    while (cube > 0) {
        seq_t digit = cube % 5;
        result += digit * power;
        power *= 10;
        cube /= 5;
    }
    
    return result;
}
