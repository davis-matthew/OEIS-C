/*
 * A172137  Number of ways to place 2 nonattacking zebras on an n X n board.
 * https://oeis.org/A172137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^4 - 9*n^2 + 40*n - 48)/2 for n >= 2, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 270, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset n=1 (and n=0) */
    if (n < 2)
        return (seq_t)0;

    /* cast n to seq_t (implementation-defined) and compute safely */
    seq_t nn  = (seq_t)n;           /* n */
    seq_t n2  = nn * nn;            /* n^2 */
    seq_t n4  = n2 * n2;            /* n^4 */

    /* rearrange to avoid intermediate underflow:
       a(n) = ((n^4 + 40*n) - (9*n^2 + 48)) / 2
       For n >= 2 the numerator is non‑negative. */
    seq_t term1 = n4 + (seq_t)40 * nn;   /* n^4 + 40*n   */
    seq_t term2 = (seq_t)9 * n2 + (seq_t)48; /* 9*n^2 + 48 */
    seq_t diff  = term1 - term2;           /* numerator */
    return diff / (seq_t)2;                /* final division */
}
