/*
 * A010946  Binomial coefficient C(30,n).
 * https://oeis.org/A010946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(30,n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t N = 30;
    if (n > N)
        return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i)
        res = res * ((seq_t)(N - i + 1)) / i;
    return res;
}
