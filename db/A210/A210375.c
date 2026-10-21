/*
 * A210375  Number of 2 X 2 matrices with all terms in {0,1,...,n} and (sum of
 *          terms) = n + 3.
 * https://oeis.org/A210375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-120 + 74*n + 15*n^2 + n^3)/6 for n > 1 a(0) = 0, a(1) =
 *            1
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t nv = (seq_t)n;
    seq_t n2 = nv * nv;          /* n^2 */
    seq_t n3 = n2 * nv;          /* n^3 */

    seq_t num = n3;                      /* start with n^3 */
    num += (seq_t)15 * n2;              /* + 15*n^2 */
    num += (seq_t)74 * nv;              /* + 74*n */
    num -= (seq_t)120;                  /* - 120 */

    return num / (seq_t)6;              /* divide by 6 */
}
