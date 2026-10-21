/*
 * A237452  Zero-based row index to irregular tables organized as successively
 *          larger square matrices.
 * https://oeis.org/A237452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = ⎣(A064866(n) – 1) / A074279(n)⎦ Here A064866(n) is the
 *            row position of n in the triangular array of squares and
 *            A074279(n) is the size of the square that contains n.
 *            Equivalently, a(n) is the zero‑based row index of n in a table
 *            built from successively larger k×k matrices filled row‑wise w...
 * Range:     u8 n <= 9455, u16 n <= 9455, u32 n <= 9455, u64 n <= 9455,
 *            u128 n <= 9455, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n == 0 we simply return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Find the smallest k such that the cumulative count
       S_k = 1^2 + 2^2 + … + k^2 is ≥ n. */
    idx_t k = 0;
    unsigned __int128 sum = 0;          /* S_k, stored in 128 bits to avoid overflow */

    while (sum < n) {
        ++k;
        sum += (unsigned __int128)k * k;
    }

    /* sum == S_k now.  Compute S_{k-1} and offset of n within the k×k matrix. */
    unsigned __int128 prev_sum = sum - (unsigned __int128)k * k;   /* S_{k-1} */
    unsigned __int128 offset   = (unsigned __int128)n - prev_sum - 1; /* 0‑based offset in the matrix */

    /* Row index in that matrix is offset / k (integer division). */
    idx_t row = (idx_t)(offset / k);

    return (seq_t)row;
}
