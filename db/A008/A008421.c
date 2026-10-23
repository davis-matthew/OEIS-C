/*
 * A008421  Crystal ball sequence for 10-dimensional cubic lattice.
 * https://oeis.org/A008421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{10} binom(10,k) * binom(n,k) * 2^k
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 19, u64 n <= 190, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t binom10[11] = {
    (seq_t)1,  (seq_t)10,  (seq_t)45,   (seq_t)120, (seq_t)210,
    (seq_t)252, (seq_t)210, (seq_t)120,  (seq_t)45,  (seq_t)10,  (seq_t)1
};
static const seq_t pow2[11] = {
    (seq_t)1, (seq_t)2,  (seq_t)4,  (seq_t)8,  (seq_t)16,
    (seq_t)32, (seq_t)64, (seq_t)128, (seq_t)256, (seq_t)512, (seq_t)1024
};

seq_t a(idx_t n) {
    seq_t sum = (seq_t)0;
    seq_t n_s = (seq_t)n;
    unsigned maxk = (n < 10 ? n : 10);
    for (unsigned k = 0; k <= maxk; ++k) {
        seq_t binom_nk = (seq_t)1;
        for (unsigned i = 1; i <= k; ++i) {
            binom_nk = binom_nk * (n_s - k + i);
            binom_nk /= i;
        }
        seq_t term = binom10[k] * pow2[k];
        sum += term * binom_nk;
    }
    return sum;
}
