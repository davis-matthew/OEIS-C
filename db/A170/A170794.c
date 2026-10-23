/*
 * A170794  a(n) = n^10*(n^2 + 1)/2.
 * https://oeis.org/A170794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10*(n^2 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 42, u128 n <= 1722,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast index to seq_t for safe arithmetic */
    seq_t u = (seq_t)n;

    /* Compute successive powers of n */
    seq_t n2 = u * u;          /* n^2 */
    seq_t n4 = n2 * n2;        /* n^4 */
    seq_t n8 = n4 * n4;        /* n^8 */
    seq_t n10 = n8 * n2;       /* n^10 */

    /* Split the division by 2 to avoid overflow.
       Exactly one of n^10 or (n^2 + 1) is even, so we can
       move the division to that factor. */
    if ((n & 1) == 0)          /* n even -> n^10 is even */
    {
        seq_t half_n10 = n10 >> 1;   /* equivalent to n^10 / 2 */
        seq_t factor = n2 + 1;       /* (n^2 + 1) */
        return half_n10 * factor;
    }
    else                        /* n odd -> (n^2 + 1) is even */
    {
        seq_t half_factor = (n2 + 1) >> 1;  /* (n^2 + 1) / 2 */
        return n10 * half_factor;
    }
}
