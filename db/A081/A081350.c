/*
 * A081350  First column in maze array of natural numbers A081349.
 * https://oeis.org/A081350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2, a(n) = (n-1)*(n+(-1)^n) for n > 1
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Convert n to seq_t once for the arithmetic */
    seq_t n_seq = (seq_t)n;
    seq_t term1 = n_seq - (seq_t)1;                /* (n-1)       */
    seq_t term2;
    /* (-1)^n is 1 for even n, -1 for odd n */
    if ((n & (idx_t)1) == 0)          /* even */
        term2 = n_seq + (seq_t)1;      /* (n+1)       */
    else                              /* odd  */
        term2 = n_seq - (seq_t)1;      /* (n-1)       */

    return term1 * term2;
}
