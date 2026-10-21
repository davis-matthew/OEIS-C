/*
 * A071289  a(n) = n*(n^2 + 1) if n is even, otherwise (n - 1/2)*(n^2 + 1).
 * https://oeis.org/A071289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+1)*(4*n-1+(-1)^n)/4
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* convert idx_t to seq_t */
    if ((n & 1) == 0) {                  /* even n  ->  a(n) = n*(n^2+1) */
        seq_t n2 = nn * nn;              /* n^2 */
        return nn * (n2 + 1);            /* n*(n^2+1) */
    } else {                             /* odd n   ->  a(n) = ((2n-1)*(n^2+1))/2 */
        seq_t n2 = nn * nn;              /* n^2 */
        seq_t part = n2 + 1;             /* n^2 + 1 */
        seq_t two_n_minus1 = (seq_t)2 * nn - (seq_t)1; /* 2n-1 */
        seq_t num = two_n_minus1 * part; /* numerator */
        return num / (seq_t)2;           /* divide by 2 (exact) */
    }
}
