/*
 * A001740  Squares written in base 5.
 * https://oeis.org/A001740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A007091(A000290(n)) - square of n written in base 5
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_squared = (seq_t)n * ((seq_t)n);
    seq_t result = 0;
    seq_t multiplier = 1;
    
    while (n_squared > 0) {
        seq_t digit = n_squared % 5;
        result = result + digit * multiplier;
        multiplier = multiplier * 10;
        n_squared = n_squared / 5;
    }
    
    return result;
}
