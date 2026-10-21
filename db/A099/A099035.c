/*
 * A099035  a(n) = (n+1)*2^(n-1) - 1.
 * https://oeis.org/A099035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*2^(n-1) - 1
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* below the offset – value irrelevant */

    seq_t power = 1;                         /* power = 2^(n-1) */
    for (idx_t i = 1; i < n; ++i)
        power *= 2;

    return ((seq_t)(n + 1)) * power - 1;
}
