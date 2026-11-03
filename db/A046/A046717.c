/*
 * A046717  a(n) = 2*a(n-1) + 3*a(n-2), a(0) = a(1) = 1.
 * https://oeis.org/A046717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2), a(0) = a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 943 (2026-11-01)
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
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)1; /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term1 = mul_small(a1, 2);
        seq_t term2 = mul_small(a0, 3);
        seq_t a2 = term1 + term2;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
