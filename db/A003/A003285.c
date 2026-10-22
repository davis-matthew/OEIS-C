/*
 * A003285  Period of continued fraction for square root of n (or 0 if n is a
 *          square).
 * https://oeis.org/A003285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Period of continued fraction for sqrt(n). If n is a perfect
 *            square, returns 0. Otherwise, uses the standard continued
 *            fraction algorithm
 * Range:     u8 n <= 2594, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt(seq_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    seq_t x = n;
    seq_t y;
    while (1) {
        y = (x + n / x) / 2;
        if (y >= x) break;
        x = y;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    
    // Check if N is a perfect square
    seq_t sq = isqrt(N);
    if (sq * sq == N) {
        return 0;
    }
    
    seq_t a0 = sq;
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_val = a0;
    seq_t period = 0;
    
    while (1) {
        m = d * a_val - m;
        d = (N - m * m) / d;
        a_val = (a0 + m) / d;
        period++;
        
        if (a_val == 2 * a0) {
            break;
        }
    }
    
    return period;
}
