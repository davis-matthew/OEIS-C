/*
 * A005225  Number of permutations of length n with equal cycles.
 * https://oeis.org/A005225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{d|n} n!/((n/d)!*d^(n/d)), each term computed as
 *            Prod_{j=1..n/d} Prod_{i=1..d-1} (j*d - i)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t k = 1; k <= n; k++) {
        if (n % k != 0) continue;
        idx_t m = n / k;
        /* n!/(m! k^m) = Prod_{j=1..m} (jk-1)!/((j-1)k)! ; all partial products grow monotonically */
        seq_t term = 1;
        for (idx_t j = 1; j <= m; j++) {
            for (idx_t i = 1; i < k; i++) {
                term *= (seq_t)(j * k - i);
            }
        }
        result += term;
    }
    return result;
}
