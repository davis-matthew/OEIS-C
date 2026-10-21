/*
 * A087327  Independence numbers for KT_2 knight on triangular board.
 * https://oeis.org/A087327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^2/2) for all n ≥ 1, except a(3) = 6 (equivalently:
 *            for n ≥ 4, a(n) = n^2/2 if n is even, a(n) = (n^2+1)/2 if n is
 *            odd )
 * Range:     u8 n <= 15, u16 n <= 267, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to arithmetic type */
    seq_t sq = nn * nn;           /* compute n^2 (fits in seq_t for n ≤ 10^4) */

    if (n == 3)                   /* special case */
        return (seq_t)6;

    /* even n: n^2 is divisible by 2 */
    if ((n & 1) == 0)
        return sq / (seq_t)2;

    /* odd n: ceil(n^2/2) = (n^2 + 1)/2 */
    return (sq + (seq_t)1) / (seq_t)2;
}
