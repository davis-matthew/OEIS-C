/*
 * A004156  Odd numbers written backwards.
 * https://oeis.org/A004156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004086(A005408(n))
 * Range:     u8 n <= 50, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t odd_num = 2 * (seq_t)n + 1;
    seq_t reversed = 0;
    seq_t temp = odd_num;
    
    while (temp > 0) {
        reversed = reversed * 10 + (temp % 10);
        temp /= 10;
    }
    
    return reversed;
}
