/*
 * A142151  a(n) = OR{k XOR (n-k): 0<=k<=n}.
 * https://oeis.org/A142151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hear, look
 * Formula:   a(n) = OR_{k=0..n} (k XOR (n-k))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        val |= ((seq_t)k) ^ ((seq_t)(n - k));
    }
    return val;
}
