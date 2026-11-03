/*
 * A198764  a(n) = 6*5^n-1.
 * https://oeis.org/A198764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*5^n-1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 985 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

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
    seq_t p   = ipow_idx(5, n);   /* compute 5^n */
    seq_t res = mul_small(p, 6); /* multiply by 6 */
    res -= 1;                    /* subtract 1 */
    return res;
}
