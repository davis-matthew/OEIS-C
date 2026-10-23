/*
 * A168566  a(n) = (n-1)*(n+2)*(n^2 + n + 2)/4.
 * https://oeis.org/A168566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n+2)*(n^2 + n + 2)/4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 360, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t nn = (seq_t)n;

    /* Compute each factor */
    seq_t term1 = nn - (seq_t)1;      /* n - 1   */
    seq_t term2 = nn + (seq_t)2;      /* n + 2   */
    seq_t term3 = nn * nn + nn + (seq_t)2; /* n^2 + n + 2 */

    /* Multiply the factors and divide by 4 */
    seq_t prod = term1 * term2;
    prod = prod * term3;

    return prod / (seq_t)4;
}
