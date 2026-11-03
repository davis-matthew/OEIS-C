/*
 * A199556  a(n) = 9*8^n + 1.
 * https://oeis.org/A199556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*8^n + 1 Recurrence: a(0) = 10, a(n) = 8*a(n-1) - 7 (n
 *            >= 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 982 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

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
    seq_t val = 10; /* a(0) */
    for (idx_t i = 0; i < n; ++i)
    {
        val = mul_small(val, 8);
        val -= 7;
    }
    return val;
}
