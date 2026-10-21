/*
 * A228887  a(n) = binomial(3*n + 1,3).
 * https://oeis.org/A228887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n + 1,3)
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 882, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t nn = (seq_t)n;

    /* Compute the three consecutive terms (3n-1), (3n), (3n+1) */
    seq_t A = 3 * nn + 1; /* 3n+1 */
    seq_t C = 3 * nn - 1; /* 3n-1 */

    /* a(n) = n * (3n+1) * (3n-1) / 2               */
    /* Use an intermediate product that never overflows
       when the final result fits in seq_t.           */
    seq_t prod = nn * A;      /* n * (3n+1)   */
    prod *= C;                /* * (3n-1)     */
    prod /= 2;                /* divide by 2   */

    return prod;
}
