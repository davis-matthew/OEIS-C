/*
 * A126885  T(n,k) = n*T(n,k-1) + k, with T(n,1) = 1, square array read by
 *          ascending antidiagonals (n >= 0, k >= 1).
 * https://oeis.org/A126885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(n',k') with n' = s-1-offset and k' = s-n', where s is
 *            the diagonal index such that s*(s+1)/2 > n. T(n,k) is defined by
 *            T(n,1)=1 and T(n,k)=n*T(n,k-1)+k
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   diag_for_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/diag_for_index.h"

static inline seq_t T_rec(idx_t n, idx_t k)
{
    /* Compute T(n,k) iteratively to avoid large intermediates. */
    seq_t t = 1;          /* T(n,1) = 1 */
    for (idx_t i = 2; i <= k; ++i)
        t = ((seq_t)n * t) + i;
    return t;
}

seq_t a(idx_t m)
{
    /* Locate the diagonal and position within it. */
    idx_t s = diag_for_index(m);
    idx_t prev_cum = (unsigned __int128)(s - 1) * s / 2;
    idx_t offset = m - prev_cum;
    idx_t n_coord = (s - 1) - offset;
    idx_t k_coord = s - n_coord;

    return T_rec(n_coord, k_coord);
}
