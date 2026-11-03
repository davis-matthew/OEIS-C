/*
 * A203819  Number of (n+1)X2 0..3 arrays with no 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16 * 9^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 200 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"

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
    /* compute 9^n in arbitrary precision (if needed) */
    seq_t pow = ipow((seq_t)9, (seq_t)n);
    /* multiply by 16 */
    return mul_small(pow, 16);
}
