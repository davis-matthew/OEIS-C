/*
 * A001738  a(n) = n^2 written in base 3.
 * https://oeis.org/A001738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A007089(A000290(n)), where A000290(n) = n^2 and
 *            A007089(n) = n written in base 3
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 242, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t square = (seq_t)n * ((seq_t)n);
    
    if (square == 0) return 0;
    
    seq_t result = 0;
    seq_t power = 1;
    
    while (square > 0) {
        seq_t digit = square % 3;
        result = result + digit * power;
        power = power * 10;
        square = square / 3;
    }
    
    return result;
}
