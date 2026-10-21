/*
 * A236632  Sum of all divisors of all positive integers <= n minus the total
 *          number of divisors of all positive integers <= n.
 * https://oeis.org/A236632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (k-1) * floor(n/k) (OEIS A236632)
 * Range:     u8 n <= 19, u16 n <= 285, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    seq_t res = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t m = (seq_t)(n / k);
        res += (seq_t)(k - 1) * m;
    }
    return res;
}
