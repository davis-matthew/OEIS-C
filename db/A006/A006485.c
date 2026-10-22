/*
 * A006485  a(n) = (2^(2^n + 1) + 1)/3.
 * https://oeis.org/A006485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(2^n + 1) + 1)/3 = 1 + 2*Sum_{j=0..2^(n-1)-1} 4^j,
 *            with the sum built by doubling: S(2K) = S(K) * (1 + 4^K), S(1) =
 *            1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s = 1;   /* S(K) = sum_{j=0}^{K-1} 4^j, starting at K = 1 */
    seq_t p = 4;   /* 4^K */
    for (idx_t i = 2; i <= n; i++) {
        s = s + s * p;
        if (i < n) p = p * p;
    }
    return 1 + 2 * s;
}
