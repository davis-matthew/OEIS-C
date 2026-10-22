/*
 * A004738  Concatenation of sequences (1,2,...,n-1,n,n-1,...,2) for n >= 2.
 * https://oeis.org/A004738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m + 1 - abs(n - 1 - m^2), where m = round(sqrt(n))
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt_seq(seq_t x) {
    if (x == 0) return 0;
    
    seq_t r = x;
    seq_t t;
    
    while (1) {
        t = (r + x / r) / 2;
        if (t >= r) return r;
        r = t;
    }
}

static seq_t round_sqrt(seq_t x) {
    seq_t r = isqrt_seq(x);
    
    if (r * r + r >= x) {
        return r;
    } else {
        return r + 1;
    }
}

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t m = round_sqrt(n_seq);
    seq_t m_sq = m * m;
    seq_t n_minus_1 = n_seq - 1;
    
    seq_t abs_diff;
    if (n_minus_1 >= m_sq) {
        abs_diff = n_minus_1 - m_sq;
    } else {
        abs_diff = m_sq - n_minus_1;
    }
    
    return m + 1 - abs_diff;
}
