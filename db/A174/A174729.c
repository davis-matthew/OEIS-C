/*
 * A174729  Triangle read by rows: T(n,k) = (1-3^n) * Eulerian(n + 1, k) +
 *          3^n.
 * https://oeis.org/A174729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less
 * Formula:   a(n) = T(r,k) with r = ⌊(sqrt(8n+1)-1)/2⌋, k = n - r(r+1)/2,
 *            T(r,k) = (1 - 3^r) * Eulerian(r + 1, k) + 3^r. Eulerian(n,k) is
 *            computed by the recurrence A(n,k) = (k+1)·A(n-1,k) +
 *            (n-k)·A(n-1,k-1), with A(1,0)=1
 * Range:     i8 n <= 6, i16 n <= 16, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   eulerian_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard integer types for idx_t and seq_t (defined by build) */
#include <stdint.h>
#include <stddef.h>
#include "../utils/eulerian_value.h"

/* main sequence function */
seq_t a(idx_t n)
{
    /* locate the row r and position k in the triangular array */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    /* number of elements before row r */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;                 /* 0 <= k <= r */

    /* compute 3^r */
    seq_t power3 = (seq_t)1;
    for (idx_t i = 0; i < r; ++i)
        power3 = power3 * (seq_t)3;

    /* Eulerian(r+1, k) */
    seq_t euler = eulerian_value(r + 1, k);

    /* (1 - 3^r) * Eulerian(r+1,k) + 3^r */
    seq_t result = ((seq_t)1 - power3) * euler + power3;
    return result;
}
