/*
 * A173962  Averages of two consecutive odd cubes; a(n) = (n^3 + (n+2)^3)/2.
 * https://oeis.org/A173962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^3 + 6*n (OEIS A173962)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 812, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is an index starting at 1; no special handling needed for n==0 here. */
    seq_t nn   = (seq_t)n;          /* cast index to seq_t */
    seq_t nn2  = nn * nn;           /* n^2 */
    seq_t nn3  = nn2 * nn;          /* n^3 */
    seq_t res  = 8 * nn3;           /* 8*n^3 */
    res += 6 * nn;                  /* +6*n */
    return res;
}
