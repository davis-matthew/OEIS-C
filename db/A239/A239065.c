/*
 * A239065  a(n) = n^3*(n^4 + n^2 - 1).
 * https://oeis.org/A239065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^7 + n^5 - n^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 565, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the offset is 1; behaviour is irrelevant below offset. */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n2 * n2;          /* n^4 */
    seq_t n5 = n4 * nn;          /* n^5 */
    seq_t n7 = n3 * n4;          /* n^7 = n^3 * n^4 */

    return n7 + n5 - n3;
}
