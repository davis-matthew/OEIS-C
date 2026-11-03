/*
 * A170440  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, if n = 0; a(n) = 47 * 46^(n-1) for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_u64_idx.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();          /* a(0) = 1 */
    /* compute 46^(n‑1) and multiply by 47 */
    return mul_small(ipow_u64_idx(46, n - 1), 47);
}
