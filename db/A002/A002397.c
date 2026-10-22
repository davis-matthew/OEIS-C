/*
 * A002397  a(n) = n! * lcm({1, 2, ..., n+1}).
 * https://oeis.org/A002397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * lcm(1, 2, ..., n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_seq(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t temp = y;
        y = x % y;
        x = temp;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    
    // Compute n!
    seq_t fact = 1;
    for (seq_t i = 2; i <= n_seq; i++) {
        fact = fact * i;
    }
    
    // Compute lcm(1, 2, ..., n+1)
    seq_t lcm_val = 1;
    for (seq_t i = 2; i <= n_seq + 1; i++) {
        seq_t g = gcd_seq(lcm_val, i);
        lcm_val = (lcm_val / g) * i;
    }
    
    return fact * lcm_val;
}
