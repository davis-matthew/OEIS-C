/*
 * A004020  Theta series of square lattice with respect to edge.
 * https://oeis.org/A004020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to x^2 + y^2 + y = n
 * Range:     u8 n <= 293, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt_seq(seq_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    seq_t x = n;
    seq_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

seq_t a(idx_t n_idx) {
    seq_t n = (seq_t)n_idx;
    seq_t count = 0;
    seq_t bound = isqrt_seq(n) + 2;
    
    // Positive y from 0 to bound
    for (seq_t y = 0; y <= bound; y++) {
        seq_t y_prod = y * (y + 1);
        if (y_prod > n) break;
        seq_t m = n - y_prod;
        seq_t sq = isqrt_seq(m);
        if (sq * sq == m) {
            count += (m == 0) ? 1 : 2;
        }
    }
    
    // Negative y: iterate ky from 1 to bound, y = -ky
    for (seq_t ky = 1; ky <= bound; ky++) {
        seq_t y_prod = ky * (ky - 1);
        if (y_prod > n) break;
        seq_t m = n - y_prod;
        seq_t sq = isqrt_seq(m);
        if (sq * sq == m) {
            count += (m == 0) ? 1 : 2;
        }
    }
    
    return count;
}
