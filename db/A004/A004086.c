/*
 * A004086  Read n backwards (referred to as R(n) in many sequences).
 * https://oeis.org/A004086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, look
 * Formula:   digit reversal of n
 * Range:     u8 n <= 102, u16 n <= 9421, u32 n <= 9421, u64 n <= 9421,
 *            u128 n <= 9421, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t num = (seq_t)n;
    seq_t reversed = 0;
    
    while (num > 0) {
        reversed = reversed * 10 + num % 10;
        num /= 10;
    }
    
    return reversed;
}
