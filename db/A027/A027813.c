/*
 * A027813  a(n) = 14*(n+1)*binomial(n+5,8).
 * https://oeis.org/A027813
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*(n+1)*binomial(n+5,8)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* values below offset are undefined */
    seq_t val = binomial_small(n + 5, 8);
    val = val * (seq_t)(n + 1);
    val = val * 14;
    return val;
}
