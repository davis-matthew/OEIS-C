/*
 * A222470  Numerator sequence of the n-th convergent of the continued
 *          fraction 1/(1-2/(2-2/(3-2/(4-...
 * https://oeis.org/A222470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = n*a(n-1) - 2*a(n-2), a(0)=0, a(1)=1 (index starts at 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 1 is returned immediately when n == 1 */
    seq_t prev2 = 0;   /* a(0) */
    seq_t prev1 = 1;   /* a(1) */

    /* Compute successive terms up to a(n) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t curr = (seq_t)i * prev1 - (seq_t)2 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
