/*
 * A070263  Triangle T(n,k), n >= 0, 1 <= k <= 2^n, read by rows, giving
 *          minimal distance-sum of any set of k binary vectors of length n.
 * https://oeis.org/A070263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A070263 – Minimal sum of pairwise Hamming distances among k distinct n‑bit vectors.
   For an index n = 0,1,2,… the sequence is given by reading the triangle T(r,k)
   row‑wise.  The position n corresponds to row r = floor_log2(n+1)
   and column k = n - (2^r - 1) + 1.  The minimal sum is obtained by
   taking the lexicographic binary representations 0 … k-1 (padded to any
   length ≥ ceil_log2(k)) and summing the products of ones and zeros in each
   bit position.  This gives a closed‑form computation requiring only integer
   arithmetic. */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* compute row r and column k */
    idx_t r   = floor_log2_uint64(n + 1u);
    idx_t prev = ((idx_t)1u << r) - 1u;
    idx_t k    = n - prev + 1u;       /* k ∈ [1, 2^r] */

    if (k == 1u)                     /* single vector ⇒ no pairwise distance */
        return 0;

    seq_t sum  = 0;
    idx_t bits = floor_log2_uint64(k - 1u);   /* highest bit that can be 1 */

    for (idx_t j = 0; j <= bits; ++j) {
        seq_t ones = 0;

        if (j < 63u) {
            idx_t block = ((idx_t)1u << (j + 1u));
            idx_t half  = block >> 1u;
            idx_t full  = k / block;
            ones = full * half;
            idx_t rem = k % block;
            if (rem > half)
                ones += rem - half;
        } else { /* j == 63 */
            if (k > ((idx_t)1u << 63u))
                ones = k - ((idx_t)1u << 63u);
            else
                ones = 0;
        }

        seq_t zeros = (seq_t)k - ones;
        sum += ones * zeros;
    }

    return sum;
}
