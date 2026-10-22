/*
 * A001614  Connell sequence: 1 odd, 2 even, 3 odd, ...
 * https://oeis.org/A001614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabl
 * Formula:   a(n) = 2*n - floor((1 + sqrt(8*n-7))/2)
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt_seq(seq_t x) {
    if (x == 0) return 0;
    seq_t s = x;
    seq_t prev = 0;
    while (s != prev) {
        prev = s;
        s = (s + x / s) / 2;
    }
    return s;
}

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t inner = 8 * n_seq - 7;
    seq_t sqrt_inner = isqrt_seq(inner);
    return 2 * n_seq - (1 + sqrt_inner) / 2;
}
