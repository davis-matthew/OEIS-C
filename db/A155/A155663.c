/*
 * A155663  a(n) = 9^n + 8^n - 1.
 * https://oeis.org/A155663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n + 8^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 958 (2026-11-01)
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
    seq_t pow9 = (seq_t)1;
    seq_t pow8 = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow9 = mul_small(pow9, 9);
    for (idx_t i = 1; i <= n; ++i)
        pow8 = mul_small(pow8, 8);
    return (pow9 + pow8) - (seq_t)1;
}
