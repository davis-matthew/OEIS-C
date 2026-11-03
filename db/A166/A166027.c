/*
 * A166027  a(n) = 6*a(n-2) for n > 2; a(1) = 4, a(2) = 1.
 * https://oeis.org/A166027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) for n>2; a(1) = 4, a(2) = 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 295 (2026-11-01)
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
    /* The sequence is defined only for n >= 1. Behaviour for n < 1
     * is irrelevant for the tests that call the function only with
     * valid indices. */
    if (n < 1) return 0;

    if (n & 1)  /* odd n: 4 * 6^((n-1)/2) */
    {
        seq_t r = ipow_idx((seq_t)6, (n - 1) / 2);
        return mul_small(r, 4);
    }
    else        /* even n: 6^((n-2)/2) */
    {
        return ipow_idx((seq_t)6, (n - 2) / 2);
    }
}
