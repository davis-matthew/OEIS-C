/*
 * A226514  Column 3 of array in A226513.
 * https://oeis.org/A226514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n^2 + 8*n + 13)
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t n2   = sn * sn;                     /* n^2          */
    seq_t expr = n2 + (seq_t)8 * sn + (seq_t)13; /* n^2 + 8n + 13 */
    return (sn + (seq_t)1) * expr;
}
