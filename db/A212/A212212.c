/*
 * A212212  Array read by antidiagonals: pi(n) + pi(k) - pi(n+k), where pi() =
 *          A000720.
 * https://oeis.org/A212212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, nice
 * Formula:   a(n) = pi(i) + pi(j) - pi(i+j). where (i,j) are the antidiagonal
 *            coordinates of the n-th element in the array read by
 *            antidiagonals. i+j is the diagonal index
 * Range:     i8 n <= 4950, i16 n <= 4950, i32 n <= 4950, i64 n <= 4950,
 *            i128 n <= 4950, big any n
 * Helpers:   pi_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pi_leq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* protect against 0; offset is 1 */

    /* Find the diagonal index s such that n ≤ s·(s+1)/2. */
    idx_t s = 1;
    while ((s * (s + 1u)) / 2u < n) ++s;

    /* Number of entries before this diagonal. */
    idx_t tri_prev = (s - 1u) * s / 2u;

    /* Position inside the current diagonal (0‑based). */
    idx_t d = n - tri_prev - 1u;

    /* Antidiagonal coordinates: (i, j). */
    idx_t i = d + 1u;
    idx_t j = s - d;

    /* Compute the prime counting function values. */
    seq_t pi_i   = pi_leq(i);
    seq_t pi_j   = pi_leq(j);
    seq_t pi_sum = pi_leq(i + j);

    return pi_i + pi_j - pi_sum;
}
