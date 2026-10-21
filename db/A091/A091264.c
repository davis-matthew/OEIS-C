/*
 * A091264  Matrix defined by a(n,k) = 2^n + (k-1), read by antidiagonals.
 * https://oeis.org/A091264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the sequence obtained by reading the infinite matrix
 *            defined by a(n,k) = 2^n + (k-1) (with n ≥ 0, k ≥ 0) along its
 *            anti‑diagonals, from upper‑left to lower‑right. For each integer
 *            n ≥ 0 this implementation converts n into the corresponding
 *            matrix position (row, k) and evaluates 2^row + (k-1)
 * Range:     u8 n <= 44, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d such that the first term of diagonal d+1
       is strictly after index n.
       The cumulative number of terms up to (and including) diagonal d
       equals (d+1)(d+2)/2. */
    idx_t d = 0;
    for (;; ++d)
    {
        /* Use 128‑bit arithmetic to avoid overflow for very large n. */
        __uint128_t cnt = (__uint128_t)(d + 1) * (d + 2) / 2;
        if (cnt > n)
            break;
    }

    /* Number of terms before diagonal d: d·(d+1)/2. */
    idx_t prev = (idx_t)(((__uint128_t)d * (d + 1)) / 2);

    /* Position inside diagonal d (0…d). */
    idx_t offset = n - prev;

    /* Matrix row index (n) and column offset (k). */
    idx_t row = d - offset;
    idx_t k   = offset;

    /* Compute the value 2^row + (k-1).  For k==0 we subtract one. */
    seq_t val = pow2(row);
    if (k == 0)
        val -= 1;
    else
        val += (seq_t)(k - 1);

    return val;
}
