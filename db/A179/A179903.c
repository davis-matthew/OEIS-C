/*
 * A179903  (1, 3, 5, 7, 9, ...) convolved with (1, 0, 3, 5, 7, 9, ...).
 * https://oeis.org/A179903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = (2*n^3 + n + 6) / 3 for n >= 1 This follows the
 *            convolution definition and matches all listed terms
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for the offset */
    if (n == 0)
        return (seq_t)1;

    seq_t tn  = (seq_t)n;        /* n */
    seq_t tn2 = tn * tn;         /* n^2 */
    seq_t tn3 = tn2 * tn;        /* n^3 */

    /* Compute (2*n^3 + n + 6) / 3 in a single integer division. */
    return (((seq_t)2 * tn3) + tn + (seq_t)6) / (seq_t)3;
}
