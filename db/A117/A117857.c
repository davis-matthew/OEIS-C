/*
 * A117857  Number of palindromes of length n (in base 5).
 * https://oeis.org/A117857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*5^floor((n-1)/2)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-01)
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
    if (n == 0) return 0;
    idx_t k = (n - 1) / 2;                 /* exponent 5^k   */
    seq_t p = ipow_idx((seq_t)5, k);         /* compute 5^k   */
    seq_t res = mul_small(p, 4);            /* multiply by 4  */
    return res;
}
