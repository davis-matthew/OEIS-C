/*
 * A138586  a(1) = 1; a(n) = a(n-1) + (n!)^7.
 * https://oeis.org/A138586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} (k!)^7
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

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
    if (n < 1) return 0;
    seq_t sum  = 0;
    seq_t fact = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        fact = mul_small(fact, (unsigned int)k);   /* fact *= k */
        seq_t term = ipow_i32(fact, 7);              /* (k!)^7 */
        sum += term;
    }
    return sum;
}
