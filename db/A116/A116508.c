/*
 * A116508  a(n) = C( C(n,2), n).
 * https://oeis.org/A116508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C( C(n,2), n )
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C(n,2) = n*(n-1)/2,  and C(m,n) = m!/(n!(m-n)!) computed multiplicatively. */

seq_t a(idx_t n)
{
    /* 0 is defined to be 1 (empty graph) */
    if (n == 0)
        return (seq_t)1;

    /* m = C(n,2) expressed in seq_t to avoid overflow of idx_t */
    seq_t m = (seq_t)n * ((seq_t)n - 1) / 2;

    /* For n = 1 or 2 we have m < n, so the binomial coefficient is 0. */
    if (m < (seq_t)n)
        return (seq_t)0;

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Numerator term: m - i + 1  (the descending product from m). */
        seq_t numerator = m - (seq_t)i + 1;
        /* Multiply then divide; the division is exact. */
        res = res * numerator / (seq_t)i;
    }
    return res;
}
