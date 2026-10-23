/*
 * A104402  Matrix inverse of triangle A091491, read by rows.
 * https://oeis.org/A104402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where (r,k) is the position of n in the row-major
 *            read of the triangle defined by T(n,k) = (-1)^(n-k) * (C(k, n-k)
 *            + C(k+1, n-k-1)) for 0 ≤ k ≤ n
 * Range:     i8 n <= 85, i16 n <= 316, i32 n <= 1162, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   binom_small, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"  /* provides int_sqrt(idx_t) */
#include "../utils/binom_small.h"  /* provides binom_small(idx_t N, idx_t K) */

seq_t a(idx_t n)
{
    /* Determine the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = (int_sqrt((n << 3) + 1) - 1) >> 1;           /* r = floor((sqrt(8n+1)-1)/2) */
    idx_t row_start = (r * (r + 1)) >> 1;                  /* position of first element of row r */
    idx_t k = n - row_start;                               /* column within the row */

    /* Compute T(r,k).  The binomial function binom_small is safe for
       the small values of r,k that arise (r ≤ ~4472 for n≈10^7). */
    idx_t N = r;
    idx_t K = k;
    seq_t sign = ((N - K) & 1ULL) ? (seq_t)-1 : (seq_t)1;
    seq_t c1 = 0, c2 = 0;

    if (N >= K)
        c1 = binom_small(K, N - K);        /* C(k, n-k) */
    if (N > K)
        c2 = binom_small(K + 1, N - K - 1); /* C(k+1, n-k-1) */

    return sign * (c1 + c2);
}
