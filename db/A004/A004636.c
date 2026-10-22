/*
 * A004636  Cubes written in base 6.
 * https://oeis.org/A004636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007092(n^3), converting n^3 to base 6
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 356, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_cubed = (seq_t)n * (seq_t)n * (seq_t)n;
    seq_t result = 0;
    seq_t multiplier = 1;
    
    while (n_cubed > 0) {
        seq_t digit = n_cubed % 6;
        result = result + digit * multiplier;
        multiplier = multiplier * 10;
        n_cubed = n_cubed / 6;
    }
    
    return result;
}
