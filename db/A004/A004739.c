/*
 * A004739  Concatenation of sequences
 *          (1,2,2,...,n-1,n-1,n,n,n-1,n-1,...,2,2,1) for n >= 1.
 * https://oeis.org/A004739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = v + (2*v-1)*(t^2-n) + t, where t = floor(sqrt(n) - 1/2) +
 *            1 and v = floor((n-1)/t) - t + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static uint64_t isqrt_u64(uint64_t n) {
    if (n == 0) return 0;
    uint64_t x = n;
    uint64_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

seq_t a(idx_t n) {
    uint64_t isqrt_n = isqrt_u64((uint64_t)n);
    
    idx_t t;
    if ((uint64_t)n >= isqrt_n * isqrt_n + isqrt_n + 1) {
        t = (idx_t)(isqrt_n + 1);
    } else {
        t = isqrt_n;
    }
    
    idx_t v = (n - 1) / t - t + 1;
    
    seq_t v_seq = (seq_t)v;
    seq_t t_seq = (seq_t)t;
    seq_t n_seq = (seq_t)n;
    
    seq_t t_squared = t_seq * t_seq;
    seq_t diff = t_squared - n_seq;
    seq_t coeff = 2 * v_seq - 1;
    seq_t product = coeff * diff;
    
    return v_seq + product + t_seq;
}
