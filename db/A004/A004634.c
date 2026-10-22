/*
 * A004634  Cubes written in base 4.
 * https://oeis.org/A004634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007090(n^3)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 101, u64 n <= 8191,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;
    seq_t result = 0;
    seq_t power = 1;
    
    while (cube > 0) {
        seq_t digit = cube % 4;
        result = digit * power + result;
        power *= 10;
        cube /= 4;
    }
    
    return result;
}
