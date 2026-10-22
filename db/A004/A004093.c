/*
 * A004093  Even numbers written backwards.
 * https://oeis.org/A004093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A004086(2*n), digit reversal of the even number 2*n
 * Range:     u8 n <= 51, u16 n <= 5003, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t even = (seq_t)n * 2;
    seq_t reversed = 0;
    seq_t temp = even;
    
    while (temp > 0) {
        reversed = reversed * 10 + (temp % 10);
        temp /= 10;
    }
    
    return reversed;
}
