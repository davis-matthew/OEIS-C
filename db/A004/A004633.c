/*
 * A004633  Cubes written in base 3.
 * https://oeis.org/A004633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007089(n^3)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 38, u64 n <= 1324,
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
        result = result + (cube % 3) * multiplier;
        multiplier = multiplier * 10;
        cube = cube / 3;
    }
    
    return result;
}
