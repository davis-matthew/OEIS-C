/*
 * A204133  Symmetric matrix based on f(i,j)=(2^(i-1) if i=j and 1 otherwise),
 *          by antidiagonals.
 * https://oeis.org/A204133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 except when the entry lies on the main diagonal of the
 *            antidiagonal traversal of the matrix f(i,j) defined by f(i,i) =
 *            2^(i-1) and f(i,j)=1 for i≠j. The pair (i,j) is found from n by
 *            solving n = (s-1)*(s-2)/2 + i where s = i + j
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* For n>=1. If an unexpected n==0 is passed, return 0. */
    if (n == 0) return (seq_t)0;

    /* Find s such that s*(s-1)/2 >= n. */
    idx_t s = 2;
    while ((unsigned __int128)s * (s - 1) / 2 < n) {
        ++s;
    }

    /* Cumulated number of terms before this anti‑diagonal. */
    unsigned __int128 prev = (unsigned __int128)(s - 1) * (s - 2) / 2;
    idx_t pos = (idx_t)n - (idx_t)prev;   /* 1 ≤ pos ≤ s‑1 */

    idx_t i = pos;
    idx_t j = s - pos;

    if (i == j) {
        return pow2(i - 1);   /* pow2 is the utility for 2^k */
    } else {
        return (seq_t)1;
    }
}
