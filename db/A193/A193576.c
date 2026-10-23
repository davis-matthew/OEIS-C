/*
 * A193576  a(n) = T(n)^3 + n^3 where T(n) is a triangular number.
 * https://oeis.org/A193576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 * (n^3 + 3*n^2 + 3*n + 9)) / 8 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 56, u64 n <= 2298,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard for non-positive indices – no values are defined there. */
    if (n == 0) return 0;

    /* Convert to seq_t before all operations. */
    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;          /* n^2 */
    seq_t n3 = n2 * n1;          /* n^3 */

    /* Compute  n^3 + 3*n^2 + 3*n + 9  */
    seq_t s = n3 + (n2 * 3) + (n1 * 3) + 9;

    /* Divide early to avoid overflow:
         if n is even, n^3 is divisible by 8,
         else the bracketed term s is divisible by 8. */
    if ((n & 1ULL) == 0ULL) {
        /* n even – n^3/8 is integral. */
        return (n3 / 8) * s;
    } else {
        /* n odd – s/8 is integral. */
        return (s / 8) * n3;
    }
}
