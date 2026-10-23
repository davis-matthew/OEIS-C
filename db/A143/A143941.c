/*
 * A143941  The Wiener index of a chain of n triangles (i.e., joined like
 *          VVV..VV; here V is a triangle!).
 * https://oeis.org/A143941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (1 + 6*n + 2*n^2) / 3 (offset 1)
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset starts at 1; undefined for 0 */

    seq_t nn = (seq_t)n;         /* cast index to seq_t */
    seq_t temp = 1 + 6 * nn + 2 * nn * nn;    /* compute 1 + 6n + 2n^2  */
    seq_t result = nn * temp / 3;             /* multiply by n and divide by 3 */
    return result;
}
