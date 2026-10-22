/*
 * A001739  Squares written in base 4.
 * https://oeis.org/A001739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A007090(A000290(n))
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t result = 0;
    seq_t power = 1;
    
    while (n_sq > 0) {
        seq_t digit = n_sq % 4;
        result = result + digit * power;
        power = power * 10;
        n_sq = n_sq / 4;
    }
    
    return result;
}
