/*
 * A176711  a(n) = 16n^4 + 64n^3 + 104n^2 + 80n + 21.
 * https://oeis.org/A176711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16n^4 + 64n^3 + 104n^2 + 80n + 21
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 126, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* use seq_t for all operations   */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */

    seq_t term1 = n4 * (seq_t)16;    /* 16 n^4 */
    seq_t term2 = n3 * (seq_t)64;    /* 64 n^3 */
    seq_t term3 = n2 * (seq_t)104;   /* 104 n^2 */
    seq_t term4 = nn * (seq_t)80;    /* 80 n    */
    seq_t term5 = (seq_t)21;         /* constant 21 */

    return term1 + term2 + term3 + term4 + term5;
}
