/*
 * A049788  a(n) = T(n,n-3), array T as in A049783.
 * https://oeis.org/A049788
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n-6} mod(n-3, floor((n-6)/j))
 * Range:     u8 n <= 181, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 6) return 0;
    idx_t n_minus3 = n - 3;
    idx_t n_minus6 = n - 6;
    seq_t sum = 0;
    for (idx_t j = 1; j <= n_minus6; ++j) {
        idx_t m = n_minus6 / j;          /* m >= 1 */
        seq_t term = (seq_t)(n_minus3 % m);
        sum += term;
    }
    return sum;
}
