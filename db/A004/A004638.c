/*
 * A004638  Cubes written in base 8.
 * https://oeis.org/A004638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007094(n^3)
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 837, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t cube = n_seq * n_seq * n_seq;
    
    seq_t result = 0;
    seq_t multiplier = 1;
    
    while (cube > 0) {
        seq_t digit = cube % 8;
        result = result + digit * multiplier;
        multiplier = multiplier * 10;
        cube = cube / 8;
    }
    
    return result;
}
