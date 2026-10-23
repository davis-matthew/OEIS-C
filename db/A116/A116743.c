/*
 * A116743  Number of permutations of length n which avoid the patterns 3421,
 *          4123, 4213.
 * https://oeis.org/A116743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 7*a(n-1) -
 *            17*a(n-2) + 21*a(n-3) - 16*a(n-4) + 6*a(n-5) - a(n-6) * with
 *            initial terms * a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=75,
 *            a(6)=267 * This follows from the generating function * A(x) =
 *            -x(x^3-2x^2+3x-1)(x-1)^2 / ((x^2-x+1)(x^4-5x^3+10x^2-6x+1))...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle out-of-range indices quietly. */
    if (n < 1) return (seq_t)0;

    /* Return one of the six initial values directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)21;
    if (n == 5) return (seq_t)75;
    if (n == 6) return (seq_t)267;

    /* For n >= 7 compute iteratively using the recurrence. */
    seq_t prev[6];                 /* prev[0] = a_{i-1}, prev[5] = a_{i-6} */
    prev[0] = (seq_t)267;          /* a(6) */
    prev[1] = (seq_t)75;           /* a(5) */
    prev[2] = (seq_t)21;           /* a(4) */
    prev[3] = (seq_t)6;            /* a(3) */
    prev[4] = (seq_t)2;            /* a(2) */
    prev[5] = (seq_t)1;            /* a(1) */

    seq_t curr = (seq_t)0;
    for (idx_t i = 7; i <= n; ++i) {
        curr = 7*prev[0] - 17*prev[1] + 21*prev[2] - 16*prev[3] + 6*prev[4] - prev[5];
        /* Shift the window for the next iteration. */
        prev[5] = prev[4];
        prev[4] = prev[3];
        prev[3] = prev[2];
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = curr;
    }
    return curr;
}
