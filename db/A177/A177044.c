/*
 * A177044  a(n) = 103*(n-1)-a(n-1) with n>1, a(1)=38.
 * https://oeis.org/A177044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (103 + 49*(-1)^(n-1) + 206*(n-1)) / 4
 * Range:     u8 n <= 1, u16 n <= 319, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Base case */
    if (n == 1) return (seq_t)38;

    /* Compute the numerator: 103 + 206*(n-1) ± 49 depending on parity */
    seq_t num = (seq_t)103 + (seq_t)206 * (n - 1);

    if (n & 1)          /* n is odd: add 49 */
        num += (seq_t)49;
    else                /* n is even: subtract 49 */
        num -= (seq_t)49;

    /* The result is always divisible by 4 */
    return num / (seq_t)4;
}
