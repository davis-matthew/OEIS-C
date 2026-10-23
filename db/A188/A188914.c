/*
 * A188914  a(n) = n*n! + 1 = (n+1)! - n! + 1.
 * https://oeis.org/A188914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*n! + 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A188914 for index n >= 0. */
seq_t a(idx_t n)
{
    seq_t fact = 1;          /* fact = 0! = 1 */
    idx_t i;                 /* loop counter */

    /* Compute n! iteratively */
    for (i = 1; i <= n; ++i)
        fact = fact * (seq_t)i;

    /* a(n) = n * n! + 1 */
    return (seq_t)n * fact + 1;
}
