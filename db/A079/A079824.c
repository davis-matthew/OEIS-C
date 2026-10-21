/*
 * A079824  Sum of numbers in n-th upward diagonal of triangle in A079823.
 * https://oeis.org/A079824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   * a(2*n) = (n/6)*(7*n^2 + 3*n + 2) * a(2*n-1) = (n/6)*(7*n^2 -
 *            6*n + 5) * (implemented with integer arithmetic using the same
 *            expressions)
 * Range:     u8 n <= 11, u16 n <= 76, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, but harmless if called */
    if ((n & 1) == 0) {            /* even index: a(2*m) */
        seq_t m = (seq_t)(n / 2);
        return (m * (7 * m * m + 3 * m + 2)) / 6;
    } else {                        /* odd index: a(2*m-1) */
        seq_t m = (seq_t)((n + 1) / 2);   /* m = (n+1)/2  */
        return (m * (7 * m * m - 6 * m + 5)) / 6;
    }
}
