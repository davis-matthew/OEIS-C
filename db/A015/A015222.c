/*
 * A015222  Even square pyramidal numbers.
 * https://oeis.org/A015222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + 4*n - (-1)^n)*(2 + 4*n - (-1)^n)*(1 + 4*n -
 *            (-1)^n)/24 Equivalent to a(n) = k(k+1)(2k+1)/6 with k =
 *            4*floor(n/2) + (n%2?3:0)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the suitable k for the n-th even square pyramidal number */
    seq_t k = (seq_t)4 * (seq_t)(n >> 1);   /* 4*floor(n/2)   */
    if (n & 1U)                             /* if n is odd    */
        k += (seq_t)3;                      /* +3             */

    seq_t k1 = k + (seq_t)1;                /* k+1             */
    seq_t k2 = (seq_t)2 * k + (seq_t)1;     /* 2k+1            */

    /* Compute k * (k+1) * (2k+1) / 6 */
    seq_t prod = k;
    prod *= k1;
    prod *= k2;
    prod /= (seq_t)6;

    return prod;
}
