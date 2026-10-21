/*
 * A054925  a(n) = ceiling(n*(n-1)/4).
 * https://oeis.org/A054925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*(n-1)/4)
 * Range:     u8 n <= 16, u16 n <= 440, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset case n == 0 explicitly */
    if (n == 0) {
        return 0;
    }

    /* cast index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn - 1);   /* n*(n-1) */
    return (prod + (seq_t)3) / 4; /* ceil division by 4 */
}
