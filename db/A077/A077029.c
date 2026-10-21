/*
 * A077029  Rectangle R(i,j) read by ascending antidiagonals: column j has j-1
 *          zeros followed by numbers congruent to 1 mod j-1.
 * https://oeis.org/A077029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = R(i,j) with i,j derived from the n‑th element in the
 *            antidiagonal enumeration of the matrix R defined by R(i,j) =
 *            (i-j)*(j-1)+1 for 1≤j≤i = 0 otherwise. The function inverts the
 *            triangular indexing and evaluates R(i,j)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the diagonal m such that (m-1)*m/2 ≥ n and (m-2)*(m-1)/2 < n. */
    idx_t lo = 1, hi = 2;
    while ((unsigned __int128)hi * (hi - 1) / 2 < n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t m = lo; /* current diagonal (sum i+j = m) */

    /* Previous total number of entries before this diagonal. */
    idx_t prev = (m < 2) ? 0
                        : (unsigned __int128)(m - 2) * (m - 1) / 2;

    /* Position within the current diagonal (0‑based). */
    idx_t offset = n - prev - 1;

    /* Determine row (i) and column (j). Bottom‑to‑top order in the diagonal. */
    idx_t i = m - 1 - offset;
    idx_t j = m - i;

    if (j > i) return 0;

    seq_t term = (seq_t)(i - j) * (seq_t)(j - 1) + 1;
    return term;
}
