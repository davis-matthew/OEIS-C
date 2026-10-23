/*
 * A145923  Second bisection of A061041: a(n) = A061041(2n+1) =
 *          (2*n+1)*(2*n+9).
 * https://oeis.org/A145923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (2*n+1)*(2*n+9)
 * Range:     u8 n <= 5, u16 n <= 125, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m  = (seq_t)n;        /* cast index to sequence type */
    seq_t l  = 2 * m + 1;       /* 2*n + 1  */
    seq_t r  = 2 * m + 9;       /* 2*n + 9  */
    return l * r;               /* (2*n+1)*(2*n+9) */
}
