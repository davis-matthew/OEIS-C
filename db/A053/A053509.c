/*
 * A053509  a(n) = binomial(n-1,4)*n^(n-5).
 * https://oeis.org/A053509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n-1,4)*n^(n-5)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 5) return (seq_t)0;
    seq_t binom = ((seq_t)(n - 1) * (n - 2)) * (n - 3) * (n - 4);
    binom = binom / (seq_t)24;
    seq_t power = pow_seq((seq_t)n, n - 5);
    return binom * power;
}
