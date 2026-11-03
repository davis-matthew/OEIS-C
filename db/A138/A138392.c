/*
 * A138392  Hankel transform of A062992 with interpolated zeros.
 * https://oeis.org/A138392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(comb(n+1,2)+0^n-1)*(2+floor((n+1)/2)-0^n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"

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
    if (n == 0) return 1;
    seq_t exp = (seq_t)n * ((seq_t)n + 1) / 2 - 1;
    seq_t pv  = pow2((idx_t)exp);
    unsigned int factor = 2 + (unsigned int)((n + 1) / 2);
    return mul_small(pv, factor);
}
