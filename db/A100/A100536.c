/*
 * A100536  a(n) = 3*n^2 - 2.
 * https://oeis.org/A100536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 - 2
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-indexed; for n >= 1 the value is 3*n^2 - 2 */
    seq_t nn = (seq_t)n;
    seq_t result = nn * nn;   /* n^2 */
    result = result * (seq_t)3; /* 3*n^2 */
    result -= (seq_t)2;          /* -2 */
    return result;
}
