/*
 * A003634  Smallest positive integer that is n times its digit sum, or 0 if
 *          no such number exists.
 * https://oeis.org/A003634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look, nice
 * Formula:   smallest positive integer k such that k = n * digit_sum(k), or 0
 *            if no such k exists
 * Range:     u8 n <= 20, u16 n <= 1882, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    
    for (seq_t d = 1; d <= 1000; d++) {
        seq_t k = n_seq * d;
        
        seq_t digit_sum = 0;
        seq_t temp = k;
        while (temp > 0) {
            digit_sum += temp % 10;
            temp /= 10;
        }
        
        if (digit_sum == d) {
            return k;
        }
    }
    
    return 0;
}
