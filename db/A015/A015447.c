/*
 * A015447  Generalized Fibonacci numbers: a(n) = a(n-1) + 11*a(n-2).
 * https://oeis.org/A015447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 11*a(n-2) (a(0) = 1, a(1) = 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 925 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    seq_t a0 = (seq_t)1;            /* a(0) */
    seq_t a1 = (seq_t)1;            /* a(1) */
    seq_t ai;
    for (idx_t i = 2; i <= n; ++i) {
        ai = a1 + mul_small(a0, 11u);
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
