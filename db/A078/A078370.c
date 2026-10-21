/*
 * A078370  a(n) = 4*(n+1)*n + 5.
 * https://oeis.org/A078370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n+1)*n + 5
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t to avoid mixed-type promotion */
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + 1);   /* n*(n+1) */
    prod = prod * 4;              /* 4*n*(n+1) */
    prod = prod + 5;              /* +5 */
    return prod;
}
