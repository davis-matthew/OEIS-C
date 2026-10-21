/*
 * A212670  a(n) = 1/15*(128*n^5 + 320*n^4 + 80*n^3 - 200*n^2 + 92*n - 15).
 * https://oeis.org/A212670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1/15*(128*n^5 + 320*n^4 + 80*n^3 - 200*n^2 + 92*n - 15)
 * Range:     u8 none, u16 n <= 3, u32 n <= 42, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t to perform all operations in the target type */
    seq_t nn = (seq_t)n;

    /* Compute successive powers of n */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */
    seq_t n5 = n4 * nn;          /* n^5 */

    /* Evaluate the polynomial using only integer operations */
    seq_t num = 128 * n5;
    num += 320 * n4;
    num +=  80 * n3;
    num -= 200 * n2;
    num +=   92 * nn;
    num -= 15;

    /* Result is always divisible by 15 */
    return num / 15;
}
