/*
 * A007069  First column of spectral array W(sqrt 2).
 * https://oeis.org/A007069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(2)*floor(sqrt(2)*n))
 * Range:     u8 n <= 8, u16 n <= 423, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt_seq(seq_t x) {
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
    seq_t n_seq = (seq_t)n;
    seq_t m1 = isqrt_seq((seq_t)2 * n_seq * n_seq);
    seq_t m2 = isqrt_seq((seq_t)2 * m1 * m1);
    return m2;
}
