/*
 * A165563  a(n) = 1 + 2*n + n^2 + 2*n^3 + n^4.
 * https://oeis.org/A165563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2*n + n^2 + 2*n^3 + n^4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to the sequence type to avoid intermediate overflow on the
       source type (idx_t is unsigned 64‑bit).  All arithmetic is then done
       in seq_t, which may be 8, 16, 32, 64 bits or arbitrary precision.*/
    seq_t n0 = (seq_t)n;
    seq_t n2 = n0 * n0;           /* n^2 */
    seq_t n3 = n2 * n0;           /* n^3 */
    seq_t n4 = n3 * n0;           /* n^4 */

    seq_t result = ((seq_t)1)          /* constant 1 */
                 + ((seq_t)2 * n0)    /* 2*n */
                 + n2                /* n^2 */
                 + ((seq_t)2 * n3)   /* 2*n^3 */
                 + n4;               /* n^4 */

    return result;
}
