/*
 * A056560  Tetrahedron with T(t,n,k)=n-k; succession of growing finite
 *          triangles with increasing values towards bottom left.
 * https://oeis.org/A056560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   C_of_n, S_of_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A056560:
 *  a(n) = A056557(n) - A056558(n)
 *  Enumerated by flattening successive finite triangles
 *  T(t,n,k) = n-k (rows of length n+1 with entries n … 0).
 *  Triangle t has rows 0…t; triangles are concatenated in order.
 */

#include <stdint.h>
#include "../utils/C_of_n.h"
#include "../utils/S_of_t.h"

/* The main sequence function */
seq_t a(idx_t n)
{
    /* Find the triangle t such that S_of_t(t-1) <= n < S_of_t(t) */
    idx_t t = 0;
    while (1) {
        __int128 sz = S_of_t(t);
        if (sz > (__int128)n) break;
        ++t;
    }

    /* S_of_t(t-1) is the number of elements before triangle t */
    idx_t sprev = 0;
    if (t > 0) {
        sprev = (idx_t)S_of_t(t - 1);
    }

    /* Position j inside triangle t (0‑based) */
    idx_t j = n - sprev;

    /* Find the row r within triangle t that contains j:
     * C_of_n(r-1) <= j < C_of_n(r)
     */
    idx_t r = 0;
    while (1) {
        __int128 cum = C_of_n(r);
        if (cum > (__int128)j) break;
        ++r;
    }

    /* Position k within that row (0‑based) */
    idx_t prev_cum = 0;
    if (r > 0) {
        prev_cum = (idx_t)C_of_n(r - 1);
    }
    idx_t k = j - prev_cum;

    /* The value at that position is r - k */
    return (seq_t)(r - k);
}
