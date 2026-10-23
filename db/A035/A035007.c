/*
 * A035007  Duplicate of A002492.
 * https://oeis.org/A035007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2 * (n-1) * n * (2n-1) / 3
 * Range:     i8 n <= 4, i16 n <= 17, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t for accurate arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute the product (n-1)*n*(2n-1) */
    seq_t prod = (nn - 1) * nn * (2 * nn - 1);

    /* Divide by 3 (always integer) and multiply by 2 */
    prod /= 3;
    prod *= 2;

    return prod;
}
