/*
 * A164203  Number of binary strings of length n with equal numbers of 00001
 *          and 10000 substrings.
 * https://oeis.org/A164203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 113 * 2^(n-7) for n >= 8 a(0) = 1, a(1) = 2, a(2) = 4,
 *            a(3) = 8, a(4) = 16, a(5) = 30, a(6) = 58, a(7) = 114
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 455 (2026-11-01)
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
    /* initial values for 0 <= n < 8 */
    static const seq_t init[8] = {1, 2, 4, 8, 16, 30, 58, 114};

    if (n < 8ULL)
        return init[(size_t)n];

    /* a(n) = 113 * 2^(n-7) for n >= 8 */
    seq_t p = pow2(n - 7ULL);  /* 2^(n-7) */
    return mul_small(p, 113U);
}
