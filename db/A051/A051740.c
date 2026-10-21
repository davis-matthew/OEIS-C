/*
 * A051740  Partial sums of A007584.
 * https://oeis.org/A051740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3,3)*(7*n+4)/4
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 263, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t ns = (seq_t)n;

    /* Compute binomial(n+3,3) = (n+1)*(n+2)*(n+3)/6 */
    seq_t bin = ns + 1;           /* n+1 */
    bin = bin * ((seq_t)n + 2);   /* (n+1)*(n+2) */
    bin = bin * ((seq_t)n + 3);   /* (n+1)*(n+2)*(n+3) */
    bin = bin / 6;                /* divide by 6 (exact) */

    /* Multiply by (7*n+4) */
    seq_t mul = (seq_t)7 * ns + 4;   /* 7*n+4 */
    bin = bin * mul;                /* numerator */

    /* Divide by 4 (exact) */
    bin = bin / 4;

    return bin;
}
