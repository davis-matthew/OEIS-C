/*
 * A004965  a(n) = ceiling(n*phi^10), where phi is the golden ratio, A001622.
 * https://oeis.org/A004965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n * phi^10), where phi = (1+sqrt(5))/2
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 773, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt_seq(seq_t x) {
    if (x < 2) return x;
    seq_t s = x;
    for (int i = 0; i < 128; i++) {
        seq_t s_new = (s + x / s) / 2;
        if (s_new >= s) break;
        s = s_new;
    }
    return s;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t n_seq = (seq_t)n;
    
    // a(n) = ceiling(n * (123 + 55*sqrt(5)) / 2)
    // = floor(n * (123 + 55*sqrt(5)) / 2) + 1
    
    // Compute floor(n*123/2)
    seq_t part_a = (n_seq * 123) / 2;
    
    // Compute floor(n*55*sqrt(5)/2) using integer square root
    // floor(n*55*sqrt(5)/2) = floor(sqrt(5*n^2*55^2) / 2)
    seq_t n_sq = n_seq * n_seq;
    seq_t disc = 5 * n_sq * 55 * 55;
    seq_t sqrt_disc = isqrt_seq(disc);
    seq_t part_b = sqrt_disc / 2;
    
    seq_t floor_result = part_a + part_b;
    
    // Account for fractional parts of n*123/2 and n*55*sqrt(5)/2
    // When n is odd, frac(n*123/2) = 1/2, so add 1 if frac(n*55*sqrt(5)/2) >= 1/2
    // frac(b) >= 1/2 iff (2*part_b + 1)^2 <= disc
    if ((n_seq & 1) == 1) {
        seq_t rhs = (2 * part_b + 1) * (2 * part_b + 1);
        if (disc >= rhs) {
            floor_result += 1;
        }
    }
    
    return floor_result + 1;
}
