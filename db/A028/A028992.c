/*
 * A028992  Even 9-gonal (or enneagonal) numbers.
 * https://oeis.org/A028992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = (1/2)*(28*(n-1)^2 + 60*(n-1) + 33 +
 *            (14*(n-1)+15)*(-1)^(n-1)). * This implementation uses only
 *            integer arithmetic on the unsigned seq_t type. * It works for
 *            all n ≥ 0 without memoisation, in O(1) time
 * Range:     u8 n <= 2, u16 n <= 67, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is defined to be 0 */
    if (n == 0)
        return 0;

    /* n >= 1 */
    /* Work with (n-1) as a seq_t for correct arithmetic */
    seq_t n1 = (seq_t)n - 1;

    /* Base part: 28*n1^2 + 60*n1 + 33 */
    seq_t base = 28 * n1 * n1;
    base += 60 * n1;
    base += 33;

    /* Offset part: 14*n1 + 15 */
    seq_t offset = 14 * n1 + 15;

    /* Combine with sign (-1)^(n-1):
     * If n is odd, sign = +1 → add offset.
     * If n is even, sign = -1 → subtract offset. 
     * For n >= 1 the base is always ≥ offset, so subtraction
     * is safe for unsigned seq_t. */
    seq_t t;
    if (n & 1)             /* n is odd → (-1)^(n-1) = +1 */
        t = base + offset;
    else                    /* n is even → (-1)^(n-1) = -1 */
        t = base - offset;

    /* Final division by 2 (exact for all n). */
    return t >> 1;
}
