/*
 * A110135  Square array of expansions of 1/sqrt(1-4x-4*k*x^2), read by
 *          antidiagonals.
 * https://oeis.org/A110135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   binom_small_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A110135: Square array of expansions of 1/sqrt(1-4x-4*k*x^2), read by antidiagonals.
   Formula:  T(n,k) = ∑_{j=0..⌊n/2⌋} C(n,j) * C(2(n-j),n) * k^j
   a(n) is T(row, col) where (row, col) are the coordinates of the n‑th element
   along the antidiagonals of the infinite array. */

/* No external headers are required; only the types idx_t and seq_t are provided. */

#include <stdint.h>  /* For uint64_t in case it is needed by the build */
#include "../utils/binom_small_local.h"

seq_t a(idx_t n) {
    /* Find the antidiagonal d such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2. */
    idx_t d = 0;
    while ((unsigned __int128)(d + 1) * (d + 2) / 2 <= n) {
        ++d;
    }

    idx_t start = (idx_t)((unsigned __int128)d * (d + 1) / 2);
    idx_t t = n - start;          /* position along the diagonal (0‑based)      */
    idx_t row = d - t;            /* row index in the array                    */
    idx_t col = t;                /* column index in the array                 */

    /* Compute the entry T(row, col). */
    idx_t maxj = row / 2;
    seq_t result = 0;
    seq_t pow_k = 1;
    seq_t k_seq = (seq_t)col;

    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t term = binom_small_local(row, j);
        term = term * binom_small_local((2 * (row - j)), row);
        term = term * pow_k;
        result += term;

        /* Update k^j for the next iteration. */
        pow_k = pow_k * k_seq;
    }

    return result;
}
