/*
 * A212683  Number of (w,x,y,z) with all terms in {1,...,n} and |x-y| = w +
 *          |y-z|.
 * https://oeis.org/A212683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 - 3*n^2 + 2*n - (n mod 2)) / 4
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = n;                     /* safe conversion to seq_t   */
    seq_t parity = ns & 1;             /* n mod 2                    */
    seq_t term = 2 * ns * ns * ns       /* 2*n^3                       */
              - 3 * ns * ns             /* -3*n^2                      */
              + 2 * ns;                 /* +2*n                        */
    term -= parity;                    /* subtract (n mod 2)          */
    return term / 4;                   /* integer division            */
}
