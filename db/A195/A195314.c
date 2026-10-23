/*
 * A195314  Centered 28-gonal numbers.
 * https://oeis.org/A195314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*n^2 - 14*n + 1
 * Range:     u8 n <= 4, u16 n <= 68, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute 14*n*(n-1) + 1 for the centered 28-gonal number */
    seq_t val = nn * (nn - 1);   /* n*(n-1) */
    val = val * 14;              /* 14*n*(n-1) */
    val = val + 1;               /* +1 */
    return val;
}
