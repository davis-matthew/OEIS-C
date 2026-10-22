/*
 * A001741  Squares written in base 6.
 * https://oeis.org/A001741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A007092(A000290(n)), n^2 written in base 6
 * Range:     u8 n <= 10, u16 n <= 88, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_squared = (seq_t)n * ((seq_t)n);
    seq_t result = 0;
    seq_t power = 1;
    
    while (n_squared > 0) {
        seq_t digit = n_squared % 6;
        result = result + digit * power;
        power = power * 10;
        n_squared = n_squared / 6;
    }
    
    return result;
}
