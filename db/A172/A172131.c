/*
 * A172131  Partial sums of floor(n^2/9) (A056838).
 * https://oeis.org/A172131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 - 15*n + 18)/54)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 2453, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;                     /* cast index to seq_t */
    seq_t num = 2 * nn * nn * nn      /* 2*n^3   */
             + 3 * nn * nn            /* +3*n^2  */
             - 15 * nn                /* -15*n   */
             + 18;                    /* +18     */
    return num / 54;                  /* floor division */
}
