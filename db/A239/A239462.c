/*
 * A239462  A239459(n) / n.
 * https://oeis.org/A239462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^{digits(n)} * n^2 + 1, where digits(n) =
 *            floor(log10(n)) + 1 (n ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 999, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* offset starts at 1, but guard against 0 */

    /* Count decimal digits of n */
    idx_t d = 0;
    for (idx_t x = n; x > 0; x /= 10)
        d++;

    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;      /* n^2 */

    /* Compute 10^d as a seq_t */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < d; ++i)
        pow10 *= 10;

    return pow10 * n2 + 1;
}
