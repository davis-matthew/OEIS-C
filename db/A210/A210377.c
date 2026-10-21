/*
 * A210377  Number of 2 X 2 matrices with all terms in {0,1,...,n} and (sum of
 *          terms) = n + 5.
 * https://oeis.org/A210377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 21*n^2 + 146*n - 504)/6 for n > 3; a(0) = 0, a(1)
 *            = 0, a(2) = 4, a(3) = 31
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 4;
    if (n == 3) return 31;

    seq_t nn = (seq_t)n;
    seq_t num = nn * nn * nn   /* n^3 */
              + 21 * nn * nn  /* + 21 n^2 */
              + 146 * nn      /* + 146 n */
              - 504;          /* - 504 */

    return num / 6;
}
