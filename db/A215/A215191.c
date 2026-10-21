/*
 * A215191  Number of arrays of 4 0..n integers with no sum of consecutive
 *          elements equal to a disjoint adjacent sum of an equal number of
 *          elements.
 * https://oeis.org/A215191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(3*n^3 + n^2 - 1)/3 for n even a(n) = (3*n^4 + n^3 - n
 *            - 3)/3 for n odd
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 209, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out of range, defined as 0 */
    seq_t N = (seq_t)n;
    seq_t val;
    if ((n & 1) == 0) {                /* even n */
        seq_t n2 = N * N;
        seq_t n3 = n2 * N;
        val = N * ( (seq_t)3 * n3 + n2 - (seq_t)1 );
        val /= (seq_t)3;
    } else {                           /* odd n */
        seq_t n2 = N * N;
        seq_t n3 = n2 * N;
        seq_t n4 = n3 * N;
        val = ( (seq_t)3 * n4 ) + n3 - N - (seq_t)3;
        val /= (seq_t)3;
    }
    return val;
}
