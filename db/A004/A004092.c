/*
 * A004092  Sum of digits of even numbers.
 * https://oeis.org/A004092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007953(2n), sum of digits of 2n
 * Range:     u8 n <= 390, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t even_num = (seq_t)n * 2;
    seq_t digit_sum = 0;
    
    while (even_num > 0) {
        digit_sum += even_num % 10;
        even_num /= 10;
    }
    
    return digit_sum;
}
