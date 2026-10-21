/*
 * A092327  a(n) = (1/12)*(n+1)*(n^3+19*n^2+118*n+228).
 * https://oeis.org/A092327
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/12)*(n+1)*(n^3+19*n^2+118*n+228) (offset -1)
 * Range:     u8 n <= 0, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Only defined for n >= -1; other values are undefined.  */
    if (n < -1) {
        return 0;          /* fallback – undefined behaviour otherwise */
    }

    seq_t nn  = (seq_t)n;           /* n cast to seq_t before any arithmetic   */
    seq_t n1  = nn + 1;             /* n+1  */

    /* Compute the polynomial term: n^3 + 19*n^2 + 118*n + 228 */
    seq_t term = nn * nn * nn          /* n^3 */
              + (seq_t)19 * nn * nn   /* 19*n^2 */
              + (seq_t)118 * nn      /* 118*n   */
              + 228;                 /* +228    */

    seq_t result = n1 * term / 12;    /* divide by 12 – always exact for integer n */
    return result;
}
