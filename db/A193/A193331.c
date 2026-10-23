/*
 * A193331  Triangle read by rows: T(n,k) = floor((k-1)*n^2/(2*k)) is an upper
 *          bound on the number of edges in the (n-k)-Turán graph.
 * https://oeis.org/A193331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, tabl, easy, look
 * Formula:   a(n) is the n-th entry of the triangle read by rows, where
 *            T(n,k) = floor((k-1)*n^2 / (2*k)) for n, k ≥ 1
 * Range:     u8 n <= 284, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compatible with any seq_t / idx_t provided by the build.  */
#include <stdint.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */

    /* Find the row r such that r(r+1)/2 ≥ n.  */
    idx_t tmp   = 1 + 8 * n;              /* 8*n fits in uint64_t for any n ≥ 1 */
    idx_t sq    = int_sqrt(tmp);          /* floor( sqrt(8*n+1) ) */
    idx_t r     = (sq - 1) / 2;           /* candidate row */

    /* Ensure the row is large enough.  */
    while (( (__int128)r * (r + 1) / 2 ) < n)
        ++r;

    /* Position within the row.  */
    idx_t prev  = (idx_t)(((__int128)r * (r - 1)) / 2); /* total entries before row r */
    idx_t k     = n - prev;                 /* 1 ≤ k ≤ r */

    /* Compute T(r,k).  Use 128‑bit arithmetic to avoid overflow.  */
    __int128 num = (__int128)(k - 1) * (__int128)r * (__int128)r;
    __int128 den = (__int128)2 * (__int128)k;
    seq_t result = (seq_t)(num / den);      /* floor division */

    return result;
}
