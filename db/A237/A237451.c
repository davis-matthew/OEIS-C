/*
 * A237451  Zero-based column index to irregular tables organized as
 *          successively larger square matrices.
 * https://oeis.org/A237451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = (n - sum_{i=1}^{k-1} i*i - 1) mod k, where k is the
 *            smallest integer with sum_{i=1}^k i*i >= n. This gives the
 *            zero‑based column index in the irregular tables of successive
 *            square matrices
 * Range:     u8 n <= 717, u16 n <= 9455, u32 n <= 9455, u64 n <= 9455,
 *            u128 n <= 9455, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t to avoid overflow when computing sums. */
    seq_t N = (seq_t)n;

    /* k is the size of the square matrix containing the n‑th element. */
    idx_t k = 1;
    seq_t cum = 1;                 /* sum of squares up to current k */

    while (cum < N)
    {
        ++k;
        cum += (seq_t)k * (seq_t)k;
    }

    /* Sum of squares of all previous matrices (size k-1). */
    seq_t cumPrev = (k == 1) ? 0 : cum - (seq_t)k * (seq_t)k;

    /* Position of n inside its k×k block, zero‑based. */
    seq_t offset = N - cumPrev - 1;

    /* Column index (zero‑based). */
    seq_t res = offset % (seq_t)k;
    return res;
}
