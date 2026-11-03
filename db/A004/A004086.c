/*
 * A004086  Read n backwards (referred to as R(n) in many sequences).
 * https://oeis.org/A004086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, look
 * Formula:   digit reversal of n
 * Range:     u8 n <= 102, u16 n <= 10006, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 91 terms through n = 9421 (2026-11-01)
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
