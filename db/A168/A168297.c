/*
 * A168297  a(n) = n^3 + (1-n)^2.
 * https://oeis.org/A168297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + (n-1)^2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* a(0) = 1 */
    seq_t nn = (seq_t)n;               /* convert index to seq_t */
    seq_t n3 = nn * nn * nn;           /* n^3, safe for n ≤ 10000 on 64‑bit types */
    seq_t n1 = nn - 1;                 /* n-1 (n ≥ 1) */
    seq_t n1sq = n1 * n1;              /* (n-1)^2 */
    return n3 + n1sq;                  /* a(n) = n^3 + (n-1)^2 */
}
