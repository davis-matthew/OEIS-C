/*
 * A014794  Squares of even octagonal numbers.
 * https://oeis.org/A014794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^2*(3*n-1)^2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* n */
    seq_t n2 = nn * nn;                  /* n^2 */
    seq_t t  = (seq_t)3 * nn - (seq_t)1; /* 3*n-1 */
    seq_t t2 = t * t;                    /* (3*n-1)^2 */
    seq_t res = n2 * (seq_t)16;          /* 16*n^2 */
    res = res * t2;                      /* 16*n^2*(3*n-1)^2 */
    return res;
}
