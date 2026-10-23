/*
 * A188916  Where squares occur in the union of squares and powers of 2.
 * https://oeis.org/A188916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊log₂(n²)⌋ − ⌊⌊log₂(n²)⌋ / 2⌋ (for n ≥ 1), with a(0)
 *            = 0. This gives the 0‑based index of the square n² in the sorted
 *            union of all squares and all powers of two
 * Range:     u8 n <= 247, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   log2_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/log2_uint128.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* 0 is the first square (index 0) */

    /* floor(log₂(n²))   – use 128‑bit product to avoid overflow */
    unsigned __int128 n_sq = (unsigned __int128)n * (unsigned __int128)n;
    idx_t l = log2_uint128(n_sq);        /* l = ⌊log₂(n²)⌋               */

    /* a(n) = n + l − ⌊l / 2⌋                */
    return (seq_t)(n + l - (l >> 1));
}
