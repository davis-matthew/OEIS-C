/*
 * A035603  Number of points of L1 norm 9 in cubic lattice Z^n.
 * https://oeis.org/A035603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^9 + 168*n^7 + 1596*n^5 + 3272*n^3 + 630*n)/2835
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 24, u64 n <= 286, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_u64(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sum = 0;
    idx_t max_i = (n - 1 < 8) ? (n - 1) : 8;
    for (idx_t i = 0; i <= max_i; ++i)
    {
        seq_t c1 = binom_u64(8, i);
        seq_t c2 = binom_u64(n - i + 8, 9);
        sum += c1 * c2;
    }
    return sum * (seq_t)2;
}
