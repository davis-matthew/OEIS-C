/*
 * A004017  Theta series of E_8 lattice with respect to deep hole.
 * https://oeis.org/A004017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16 * σ_3(n) if n is odd; a(n) = 16 * (σ_3(n) - σ_3(n/2))
 *            if n is even, where σ_3(n) = sum of d^3 for divisors d of n
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 638, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sigma3(idx_t n) {
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            seq_t d_seq = (seq_t)d;
            seq_t d3 = d_seq * d_seq * d_seq;
            result += d3;
            
            idx_t other = n / d;
            if (d != other) {
                seq_t other_seq = (seq_t)other;
                seq_t other3 = other_seq * other_seq * other_seq;
                result += other3;
            }
        }
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t s3 = sigma3(n);
    
    if (n % 2 == 1) {
        return 16 * s3;
    } else {
        seq_t s3_half = sigma3(n / 2);
        return 16 * (s3 - s3_half);
    }
}
