/*
 * A187030  Number of 5-step one or two collinear space at a time queen's
 *          tours on an n X n board summed over all starting positions.
 * https://oeis.org/A187030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 50272*n^2 - 328792*n + 534104 for n>7; table values for
 *            n≤7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 7)
    {
        static const seq_t values[8] =
        {
            0,         /* n = 0 (unused) */
            0,         /* n = 1 */
            0,         /* n = 2 */
            5120,      /* n = 3 */
            45152,     /* n = 4 */
            161256,    /* n = 5 */
            376248,    /* n = 6 */
            696992     /* n = 7 */
        };
        return values[n];
    }
    seq_t nn = (seq_t)n;
    return 50272 * nn * nn - 328792 * nn + 534104;
}
