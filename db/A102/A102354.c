/*
 * A102354  a(n) is the number of ways n can be written as k^2 * j, 0 < j <=
 *          k.
 * https://oeis.org/A102354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(k,j) : n = k^2 * j, 1 <= j <= k}
 * Range:     u8 n <= 105, u16 n <= 9788, u32 n <= 9788, u64 n <= 9788,
 *            u128 n <= 9788, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of representations of n as k^2 * j with j <= k. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t n_seq = (seq_t)n;
    seq_t count = 0;
    for (idx_t k = 1; k <= n / k; ++k) {
        seq_t k2 = (seq_t)k * (seq_t)k;            /* k^2 */
        if (n_seq % k2 == 0) {
            seq_t j = n_seq / k2;
            if (j <= (seq_t)k)
                ++count;
        }
    }
    return count;
}
