/*
 * A212274  Minimal k >= 5n such that n^2 + 2nk + k is a perfect square.
 * https://oeis.org/A212274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 52, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal k >= 5*n such that n^2 + 2*n*k + k is a perfect square */

seq_t a(idx_t n) {
    seq_t five_n = (seq_t)5 * (seq_t)n;
    seq_t d = (seq_t)2 * (seq_t)n + 1;

    for (idx_t t = 1; ; ++t) {
        seq_t prod = (seq_t)t * ((seq_t)2 * (seq_t)n + t);
        if (prod % d == 0) {
            seq_t k = prod / d;
            if (k >= five_n) {
                return k;
            }
        }
    }
}
