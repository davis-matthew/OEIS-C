/*
 * A207935  Number of n X 5 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (30 - 149*n + 10*n^2 + 250*n^3 + 65*n^4 + 4*n^5)/15 (OEIS
 *            A207935)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 69, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t N = (seq_t)n;

    seq_t p2 = N * N;          /* n^2 */
    seq_t p3 = p2 * N;         /* n^3 */
    seq_t p4 = p3 * N;         /* n^4 */
    seq_t p5 = p4 * N;         /* n^5 */

    /* Positive part of the numerator */
    seq_t pos = (seq_t)4 * p5
              + (seq_t)65 * p4
              + (seq_t)250 * p3
              + (seq_t)10 * p2
              + (seq_t)30;

    /* Negative part: 149*n */
    seq_t neg = (seq_t)149 * N;

    seq_t numerator = pos - neg;        /* guaranteed non‑negative */
    return numerator / (seq_t)15;        /* exact division */
}
