/*
 * A176145  a(n) = n*(n-3)*(n^2-7*n+14)/8.
 * https://oeis.org/A176145
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-3)*(n^2-7*n+14)/8
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 432, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) /* offset is 3; value is irrelevant for n < 3 */
        return (seq_t)0;

    /* cast index to seq_t for arbitrary precision calculations */
    seq_t sn = (seq_t)n;
    seq_t term1 = sn * (sn - (seq_t)3);

    seq_t sq = sn * sn;                     /* n^2 */
    seq_t temp = sq - ((seq_t)7 * sn) + (seq_t)14; /* n^2 - 7n + 14 */

    seq_t prod = term1 * temp;              /* n*(n-3)*(n^2-7n+14) */
    return prod / (seq_t)8;                /* guaranteed divisible by 8 */
}
