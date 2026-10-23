/*
 * A172207  Number of ways to place 3 nonattacking bishops on a 3 X n board.
 * https://oeis.org/A172207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(1)=1, a(2)=6, a(3)=26, a(n) = (9*n^3 - 45*n^2 + 106*n -
 *            108)/2 for n >= 4 (derived from the generating-function or
 *            closed form)
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 804, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly */
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)6;
    if (n == 3u) return (seq_t)26;

    /* For n >= 4 use the closed‑form expression */
    seq_t nn = (seq_t)n;           /* cast index to seq_t */
    seq_t n2 = nn * nn;            /* n^2 */
    seq_t n3 = n2 * nn;            /* n^3 */

    /* Compute the numerator using only seq_t arithmetic */
    seq_t numerator = 9 * n3 - 45 * n2 + 106 * nn - (seq_t)108;

    /* The result is guaranteed to be an integer */
    return numerator / (seq_t)2;
}
