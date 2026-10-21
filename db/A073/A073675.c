/*
 * A073675  Rearrangement of natural numbers such that a(n) is the smallest
 *          proper divisor of n not included earlier but if no such divisor
 *          exists then a(n) is the smallest proper multiple of n not included
 *          earlier, subject always to the condition that a(n) is not equal to
 *          n.
 * https://oeis.org/A073675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n if valuation_2(n) is even, otherwise a(n) = n/2
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    int v = 0;
    /* Count trailing zeros (exponent of 2) */
    while ((m & 1) == 0) {
        v++;
        m >>= 1;
    }
    if ((v & 1) == 0) {
        return (seq_t)n * 2;
    } else {
        return (seq_t)(n >> 1);  /* n/2 */
    }
}
