/*
 * A204158  Symmetric matrix based on f(i,j)=max(3i-2j, 3j-2i), by
 *          antidiagonals.
 * https://oeis.org/A204158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = max(3*i-2*j, 3*j-2*i) where (i,j) are the anti‑diagonal
 *            coordinates of n
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal number d and the position r within that diagonal. */
    idx_t d = 1;          /* diagonal index (i+j-1) */
    idx_t r = n;          /* 1‑based position inside the diagonal */

    /* Reduce r until it lies within the current diagonal. */
    while (r > d) {
        r -= d;
        ++d;
    }

    /* Convert the 1‑based position to matrix coordinates. */
    idx_t i = r;            /* row index */
    idx_t j = d - r + 1;    /* column index */

    /* Compute M(i,j) = max(3i-2j, 3j-2i) without risking underflow. */
    if (i > j) {
        return 3 * (seq_t)i - 2 * (seq_t)j;
    } else {
        return 3 * (seq_t)j - 2 * (seq_t)i;
    }
}
