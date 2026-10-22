/*
 * A007961  n written in base where place values are positive squares.
 * https://oeis.org/A007961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   n represented in base with place values k^2 for k=1,2,3,
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 120, u64 n <= 440,
 *            u128 n <= 1599, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt(idx_t n) {
    if (n == 0) return 0;
    idx_t x = n;
    idx_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    idx_t k = isqrt(n);
    seq_t result = 0;
    seq_t remaining = n_val;
    
    while (k >= 1) {
        seq_t k_seq = (seq_t)k;
        seq_t square = k_seq * k_seq;
        seq_t next_square = (k_seq + 1) * (k_seq + 1);
        
        seq_t max_digit = (next_square - 1) / square;
        seq_t digit = remaining / square;
        if (digit > max_digit) {
            digit = max_digit;
        }
        
        result = result * 10 + digit;
        remaining -= digit * square;
        
        k--;
    }
    
    return result;
}
