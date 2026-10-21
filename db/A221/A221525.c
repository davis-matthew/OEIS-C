/*
 * A221525  Number of 0..n arrays of length 5 with each element differing from
 *          at least one neighbor by 2 or more.
 * https://oeis.org/A221525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 6 For n > 2: a(n) = n^5 - n^4 - 10*n^3 + 38*n^2
 *            - 60*n + 40
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)6;

    seq_t nn = (seq_t)n;
    seq_t val = nn * nn * nn * nn * nn;                /* n^5          */
    val = val - nn * nn * nn * nn;                     /* - n^4       */
    val = val - (seq_t)10 * nn * nn * nn;              /* -10 n^3     */
    val = val + (seq_t)38 * nn * nn;                   /* +38 n^2     */
    val = val - (seq_t)60 * nn;                        /* -60 n       */
    val = val + (seq_t)40;                             /* +40         */
    return val;
}
