/*
 * A077616  Binomial transform of n^2*2^n/2.
 * https://oeis.org/A077616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n+1)*3^(n-2) (offset 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow3(idx_t e)
{
    seq_t r = (seq_t)1;
    for (idx_t i = (idx_t)0; i < e; ++i)
        r *= (seq_t)3;
    return r;
}

seq_t a(idx_t n)
{
    /* Offset is 1 – a(1) = 1 */
    if (n == (idx_t)0)
        return (seq_t)0;          /* not used, but safe */
    if (n == (idx_t)1)
        return (seq_t)1;

    seq_t term = (seq_t)n * (((seq_t)n * (seq_t)2) + (seq_t)1);
    term *= pow3(n - (idx_t)2);
    return term;
}
