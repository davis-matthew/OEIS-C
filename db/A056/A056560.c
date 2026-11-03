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
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-01)
 * Helpers:   none
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

/* internal helper: cumulative elements up to and including row n
 * C(n) = (n+1)(n+2)/2
 */
static inline __int128 C_of_n(idx_t n)
{
    __int128 N = (__int128)n + 1;        // N = n+1
    return N * (N + 1) / 2;
}

/* internal helper: cumulative elements up to and including triangle t
 * S(t) = sum_{s=0}^t (s+1)(s+2)/2
 */
static inline __int128 S_of_t(idx_t t)
{
    __int128 N = (__int128)t + 1;          // N = t+1
    __int128 sum_sq = N * (N + 1) * (2 * N + 1) / 6;   // sum of squares up to N
    __int128 sum_1  = N * (N + 1) / 2;               // sum of first N integers
    return (sum_sq + sum_1) / 2;       // S(t) as derived
}

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
