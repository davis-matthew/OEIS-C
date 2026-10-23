/*
 * A147625  Octo-factorial numbers (4).
 * https://oeis.org/A147625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0}^{n-1} (8*k+5) (Octo-factorial)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 1; for n >= 2: a(n) = (8*n-11) * a(n-1) */
    if (n <= 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
    {
        result *= (seq_t)(8 * i - 11);
    }
    return result;
}
