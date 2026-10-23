/*
 * A010931  Binomial coefficient C(15,n).
 * https://oeis.org/A010931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(15, n)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 15) return 0;

    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        int numerator = 15 - (int)n + (int)i;
        res = (res * (seq_t)numerator) / (seq_t)i;
    }
    return res;
}
