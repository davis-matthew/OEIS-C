/*
 * A071800  Number of lattice paths in the lattice [0..2n] X [0..2n] X [0..2n]
 *          which do not pass through the point (n,n,n). Number of paths
 *          through a lattice containing a "hole".
 * https://oeis.org/A071800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = s(3,2n) – s(3,n)^2 where s(3,m) = binomial(3m,m,m,m) =
 *            binomial(3m,m) * binomial(2m,m)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom(idx_t n, idx_t k) {
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / ((seq_t)i);
    }
    return res;
}

static seq_t s3(idx_t m) {
    seq_t bin1 = binom(3 * m, m);
    seq_t bin2 = binom(2 * m, m);
    return bin1 * bin2;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset starts at 1; unused for n=0 */
    seq_t s3_2n = s3(2 * n);
    seq_t s3_n  = s3(n);
    return s3_2n - s3_n * s3_n;
}
