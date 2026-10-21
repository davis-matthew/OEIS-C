/*
 * A098462  a(n) = n^n + (n+1)^n.
 * https://oeis.org/A098462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n + (n+1)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t n_seq   = (seq_t)n;
    seq_t np1_seq = (seq_t)(n + 1);
    return pow_seq_t(n_seq,   n) + pow_seq_t(np1_seq, n);
}
