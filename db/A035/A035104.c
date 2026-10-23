/*
 * A035104  First differences give (essentially) A028242.
 * https://oeis.org/A035104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5+3*(-1)^n+28*n+2*n^2)/8
 * Range:     u8 n <= 6, u16 n <= 214, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the quadratic formula in integer arithmetic.
       All intermediate results fit in seq_t before the final division. */
    seq_t n2 = (seq_t)n;           /* n as seq_t */
    seq_t val = (seq_t)5;          /* start with constant 5 */

    /* Add the 3*(-1)^n term: +3 if n even, -3 if n odd. */
    if (n & 1) {
        val -= (seq_t)3;          /* odd n: 5 - 3 = 2 */
    } else {
        val += (seq_t)3;          /* even n: 5 + 3 = 8 */
    }

    /* Add 28*n */
    val += (seq_t)28 * n2;

    /* Add 2*n^2 */
    val += (seq_t)2 * n2 * n2;

    /* Divide by 8; the expression is always divisible by 8. */
    return val / 8;
}
