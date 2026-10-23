/*
 * A163279  a(n) = (n^6 + 2n^5 + 2n^4 + n^3 + 2n)/2.
 * https://oeis.org/A163279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^6 + 2*n^5 + 2*n^4 + n^3 + 2*n)/2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 43, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote index to seq_t to avoid overflow of intermediate computations. */
    seq_t tn   = (seq_t)n;
    seq_t tn2  = tn * tn;          /* n^2 */
    seq_t tn3  = tn2 * tn;         /* n^3 */
    seq_t tn4  = tn3 * tn;         /* n^4 */
    seq_t tn5  = tn4 * tn;         /* n^5 */
    seq_t tn6  = tn5 * tn;         /* n^6 */

    /* Sum the terms of the formula.  All operations are on seq_t, so
       they automatically use the appropriate integer type (uint8_t …,
       unsigned __int128, or big‑int). */
    seq_t sum = tn6 + tn5 * (seq_t)2
                + tn4 * (seq_t)2
                + tn3
                + tn   * (seq_t)2;

    /* Exact division by 2.  The expression is always even for integer n. */
    return sum / (seq_t)2;
}
