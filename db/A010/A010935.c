/*
 * A010935  Binomial coefficient C(19,n).
 * https://oeis.org/A010935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(19, n) (zero for n > 19)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t k = 19;
    if (n > k)
        return 0; /* binomial coefficient is zero for n > k */

    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i)
        res = res * (seq_t)(k - i + 1) / (seq_t)i;

    return res;
}
