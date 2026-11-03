/*
 * A049210  a(n) = -Product_{k=0..n} (8*k-1); octo-factorial numbers.
 * https://oeis.org/A049210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=0..n} |8*k - 1| (a(0) = 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 281 (2026-11-01)
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
    if (n == 0)
        return (seq_t)1;

    seq_t prod = (seq_t)1;
    /* The factor for k = 0 is 1, so we start at k = 1. */
    for (idx_t k = 1; k <= n; ++k) {
        unsigned int term = 8u * (unsigned int)k - 1u;
        prod = mul_small(prod, term);
    }
    return prod;
}
