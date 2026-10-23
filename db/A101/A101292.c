/*
 * A101292  a(n) = n! + Sum_{i=1..n} i.
 * https://oeis.org/A101292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = n! + n*(n+1)/2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
    {
        fact *= (seq_t)i;
    }
    seq_t add = (seq_t)n * ((seq_t)n + 1) / 2;
    return fact + add;
}
