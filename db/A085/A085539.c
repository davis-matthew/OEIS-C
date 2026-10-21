/*
 * A085539  a(n) = n^6 - n^5.
 * https://oeis.org/A085539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 - n^5
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the formula gives 0, but avoid
     * unsigned underflow when computing (n-1). */
    if (n < 2) return 0;

    seq_t t = (seq_t)n;          /* n */
    seq_t n2 = t * t;            /* n^2 */
    seq_t n3 = n2 * t;           /* n^3 */
    seq_t n4 = n3 * t;           /* n^4 */
    seq_t n5 = n4 * t;           /* n^5 */
    seq_t n6 = n5 * t;           /* n^6 */

    return n6 - n5;              /* n^6 - n^5 */
}
