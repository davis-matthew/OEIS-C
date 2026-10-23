/*
 * A182789  Number of n-colorings of the 4 X 4 X 4 triangular grid.
 * https://oeis.org/A182789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)^4*(n^4-9*n^3+31*n^2-49*n+31)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 11, u64 n <= 86, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t nn     = (seq_t)n;            /* n */
    seq_t term1  = nn * (nn - 1);       /* n*(n-1) */
    seq_t n2     = nn - 2;              /* n-2 */
    seq_t n2pow4 = n2 * n2 * n2 * n2;   /* (n-2)^4 */

    /* polynomial part: n^4 - 9*n^3 + 31*n^2 - 49*n + 31 */
    seq_t poly   = nn * nn * nn * nn;      /* n^4 */
    poly = poly - 9 * (nn * nn * nn);      /* -9*n^3 */
    poly = poly + 31 * (nn * nn);          /* +31*n^2 */
    poly = poly - 49 * nn;                 /* -49*n */
    poly = poly + 31;                      /* +31 */

    return term1 * n2pow4 * poly;          /* full formula */
}
