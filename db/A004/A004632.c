/*
 * A004632  Cubes written in base 2.
 * https://oeis.org/A004632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007088(n^3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 101, u128 n <= 8191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cube = (seq_t)n;
    cube = cube * cube * cube;
    
    seq_t result = 0;
    seq_t power = 1;
    
    while (cube > 0) {
        result = result + (cube & 1) * power;
        power = power * 10;
        cube = cube >> 1;
    }
    
    return result;
}
