/*
 * A004155  Sum of digits of n-th odd number.
 * https://oeis.org/A004155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007953(2n+1), sum of digits of the n-th odd number
 * Range:     u8 n <= 607, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t odd_num = (seq_t)n * 2 + 1;
    seq_t digit_sum = 0;
    
    while (odd_num > 0) {
        digit_sum += odd_num % 10;
        odd_num /= 10;
    }
    
    return digit_sum;
}
