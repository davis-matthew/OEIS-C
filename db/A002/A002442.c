/*
 * A002442  Squares written in base 9.
 * https://oeis.org/A002442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007095(A000290(n)) - convert n^2 to base 9, read as
 *            decimal
 * Range:     u8 n <= 14, u16 n <= 208, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_sq = (seq_t)n * (seq_t)n;
    
    seq_t result = 0;
    seq_t power = 1;
    
    while (n_sq > 0) {
        seq_t digit = n_sq % 9;
        result = result + digit * power;
        power = power * 10;
        n_sq = n_sq / 9;
    }
    
    return result;
}
