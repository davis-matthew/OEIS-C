/*
 * A001637  Numbers with an even number of digits.
 * https://oeis.org/A001637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + (100^floor(log_100(110*n)) - 1)/11
 * Range:     u8 n <= 2, u16 n <= 634, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t prod = 110 * n_seq;
    
    seq_t power = 1;
    while (power * 100 <= prod) {
        power *= 100;
    }
    
    return n_seq + (power - 1) / 11;
}
