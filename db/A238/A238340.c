/*
 * A238340  Number of partitions of 4n into 4 parts.
 * https://oeis.org/A238340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(((4*n)^3 + 3*(4*n)^2)/144)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 1848, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the number of partitions of 4*n into 4 positive parts. */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;                 /* Convert index to seq_t */
    seq_t num = (seq_t)64 * N * N * N    /* (4*n)^3  = 64*n^3   */
                + (seq_t)48 * N * N;     /* + 3*(4*n)^2 = 48*n^2 */

    seq_t q = num / (seq_t)144;         /* Integer part */
    seq_t r = num % (seq_t)144;         /* Remainder */

    /* Round to nearest integer: add 1 if remainder >= 144/2 */
    if (r * (seq_t)2 >= (seq_t)144)
        return q + (seq_t)1;
    else
        return q;
}
