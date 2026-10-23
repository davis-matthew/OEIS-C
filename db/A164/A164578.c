/*
 * A164578  Integers of the form (k+1)*(2*k+1)/12.
 * https://oeis.org/A164578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (24*n^2 + 6*n - (-1)^n*(8*n+1) + 1)/4
 * Range:     u8 n <= 2, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = (24*n^2 + 14*n + 2)/4  if n is odd   */
/*              a(n) = (24*n^2 - 2*n)   /4  if n is even   */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n=0 is unused */

    seq_t nn = (seq_t)n;           /* use a seq_t for the arithmetic */
    seq_t num;

    if (nn & 1) {                  /* odd n */
        num = 24 * nn * nn + 14 * nn + 2;
    } else {                       /* even n */
        num = 24 * nn * nn - 2 * nn;
    }

    return num >> 2;               /* divide by 4 */
}
