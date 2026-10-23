/*
 * A154692  Triangle read by rows: T(n, k) = (2^(n-k)*3^k +
 *          2^k*3^(n-k))*binomial(n, k).
 * https://oeis.org/A154692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row, k) where T(n, k) = (2^(n-k)*3^k +
 *            2^k*3^(n-k))*C(n, k), and (row, k) is the position of n in the
 *            triangular reading of T
 * Range:     u8 n <= 10, u16 n <= 37, u32 n <= 124, u64 n <= 444,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_local(idx_t n, idx_t k) {
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; i++) {
        res = res * (n - k + i) / i;
    }
    return res;
}

static seq_t pow_local(seq_t base, idx_t exp) {
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)0; i < exp; i++) res *= base;
    return res;
}

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = (idx_t)0;
    while ((r + (idx_t)1) * (r + (idx_t)2) / (idx_t)2 <= n) r++;

    idx_t offset = r * (r + (idx_t)1) / (idx_t)2;
    idx_t k = n - offset;

    seq_t bin = binom_local(r, k);

    seq_t term1 = pow_local((seq_t)2, r - k) * pow_local((seq_t)3, k);
    seq_t term2 = pow_local((seq_t)2, k) * pow_local((seq_t)3, r - k);

    return (term1 + term2) * bin;
}
