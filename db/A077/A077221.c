/*
 * A077221  a(0) = 0 and then alternately even and odd numbers in increasing
 *          order such that the sum of any two successive terms is a square.
 * https://oeis.org/A077221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 8*n^2, a(2*n+1) = 8*n*(n+1)+1
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A077221 */
seq_t a(idx_t n)
{
    /* k = floor(n/2) */
    idx_t k = n >> 1;
    seq_t kk = (seq_t)k;

    if ((n & 1) == 0)            /* even index 2*n */
        return (seq_t)8 * kk * kk;
    else                          /* odd index 2*n+1 */
        return (seq_t)8 * kk * (kk + 1) + 1;
}
