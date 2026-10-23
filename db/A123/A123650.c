/*
 * A123650  a(n) = 1 + n^2 + n^3 + n^5.
 * https://oeis.org/A123650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + n^2 + n^3 + n^5
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the formula is undefined; return 0 in that case. */
    if (n == 0)
        return 0;

    seq_t x   = (seq_t)n;        /* n converted to the sequence type */
    seq_t n2  = x * x;           /* n^2 */
    seq_t n3  = n2 * x;          /* n^3 */
    seq_t n5  = n3 * n2;         /* n^5 = n^3 * n^2 */

    return 1 + n2 + n3 + n5;
}
