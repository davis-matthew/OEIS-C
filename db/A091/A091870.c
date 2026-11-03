/*
 * A091870  A trinomial transform of Fibonacci(3n).
 * https://oeis.org/A091870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 11*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 1241 (2026-11-01)
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

seq_t a(idx_t n)
{
    seq_t a0 = 0;   /* a(0) */
    seq_t a1 = 1;   /* a(1) */

    if (n == 0) return a0;
    if (n == 1) return a1;

    seq_t ai, t1, t2;
    for (idx_t i = 2; i <= n; ++i) {
        t1 = mul_small(a1, 8);
        t2 = mul_small(a0, 11);
        ai = t1 - t2;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
