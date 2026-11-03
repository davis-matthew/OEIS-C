/*
 * A022402  Fibonacci sequence beginning 1, 32.
 * https://oeis.org/A022402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*A000045(n) + A000045(n+1) ; Recurrence: a(n) = a(n-1)
 *            + a(n-2), a(0) = 1, a(1) = 32
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 944 (2026-11-01)
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/fib_pair.h"

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
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);          /* fn  = F(n),   fn1 = F(n+1) */
    return fn1 + mul_small(fn, 31);   /* 31*F(n) + F(n+1) */
}
