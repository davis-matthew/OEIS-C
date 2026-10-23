/*
 * A157615  On an n X n board, a(n) is the maximal number of squares covered
 *          by a self-avoiding path made of alternated vertical and horizontal
 *          unitary steps.
 * https://oeis.org/A157615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, more, nonn, nice
 * Formula:   a(n) = 1 for n = 1 * n^2 - n + 2 for even n * n^2 - 2n + 4 for
 *            odd n > 1 * (derived from the conjectured closed form for
 *            A157615)
 * Range:     u8 n <= 16, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The offset of the sequence is 1, so n is always >= 1
     * for defined terms.  For safety, return 0 when n == 0. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t nn = (seq_t)n;
    /* Use n & 1ULL to test parity (n is unsigned 64‑bit). */
    if ((n & 1ULL) == 0) {
        /* Even n: a(n) = n^2 - n + 2 */
        return nn * nn - nn + (seq_t)2;
    } else {
        /* Odd n > 1: a(n) = n^2 - 2n + 4 */
        return nn * nn - (seq_t)2 * nn + (seq_t)4;
    }
}
