/*
 * A071647  a(n) is the maximum number of elements among the continued
 *          fractions for n/1, n/2, n/3, n/4, ..., n/n.
 * https://oeis.org/A071647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum number of quotients in the Euclidean algorithm
 *            applied to n/k for 1 ≤ k ≤ n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently, a(n) is the maximum length of the continued‑fraction expansion of n/k among k=1…n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t max_len = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t a1 = n, b1 = k;
        seq_t len = 0;
        while (b1 != 0) {
            idx_t temp = a1 % b1;
            a1 = b1;
            b1 = temp;
            ++len;
        }
        if (len > max_len) max_len = len;
    }
    return max_len;
}
