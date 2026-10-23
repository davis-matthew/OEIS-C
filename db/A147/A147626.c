/*
 * A147626  Octo-factorial numbers (5).
 * https://oeis.org/A147626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*n-10)*a(n-1) for n>1; a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    if (n <= 1)
        return result;
    for (idx_t i = 2; i <= n; ++i)
        result = result * (seq_t)(8 * i - 10);
    return result;
}
