/*
 * A004159  Sum of digits of n^2.
 * https://oeis.org/A004159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A007953(A000290(n)); sum of digits of n^2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t n_sq = n * n;
    seq_t digit_sum = 0;
    
    while (n_sq > 0) {
        digit_sum += (seq_t)(n_sq % 10);
        n_sq /= 10;
    }
    
    return digit_sum;
}
