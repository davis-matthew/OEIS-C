/*
 * A001983  Numbers that are the sum of 2 distinct squares: of form x^2 + y^2
 *          with 0 <= x < y.
 * https://oeis.org/A001983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) is the n-th positive integer expressible as x^2 + y^2 with
 *            0 <= x < y
 * Range:     u8 n <= 87, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt(seq_t x) {
    if (x == 0) return 0;
    seq_t guess = x;
    seq_t next;
    while (1) {
        next = (guess + x / guess) / 2;
        if (next >= guess) break;
        guess = next;
    }
    return guess;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t m = 0;
    
    while (count < (seq_t)n) {
        m++;
        
        seq_t sqrt_half = isqrt(m / 2);
        
        for (seq_t x = 0; x <= sqrt_half; x++) {
            seq_t y_sq = m - x * x;
            seq_t y = isqrt(y_sq);
            
            if (y * y == y_sq && x < y) {
                count++;
                break;
            }
        }
    }
    
    return m;
}
