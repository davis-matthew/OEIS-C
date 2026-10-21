/*
 * A054333  1/256 of tenth unsigned column of triangle A053120 (T-Chebyshev,
 *          rising powers, zeros omitted).
 * https://oeis.org/A054333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*C(n+9,9) - C(n+8,8)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 40, u64 n <= 526, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        unsigned __int128 tmp = (unsigned __int128)res * (n - k + i);
        res = (seq_t)(tmp / i);
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t c9 = binom(n + 9, 9);   /* C(n+9,9) */
    seq_t c8 = binom(n + 8, 8);   /* C(n+8,8) */
    unsigned __int128 val = (unsigned __int128)c9 * 2 - (unsigned __int128)c8;
    return (seq_t)val;
}
