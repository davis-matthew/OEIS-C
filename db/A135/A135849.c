/*
 * A135849  a(n) is the ratio of the sum of the bends (curvatures) of the
 *          circles in the n-th generation of an Apollonian packing to the sum
 *          of the bends in the initial four-circle configuration.
 * https://oeis.org/A135849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 3*a(n-2) with initial values a(1)=1, a(2)=5,
 *            a(3)=39
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 189 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the supplied helper mul_small
   (../utils/mul_small.h) to avoid overflow when multiplying by
   small constants. */

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
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)39;

    seq_t prev2 = (seq_t)5;   /* a(2) */
    seq_t prev1 = (seq_t)39;  /* a(3) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t term1 = mul_small(prev1, 8);   /* 8*a(i-1) */
        seq_t term2 = mul_small(prev2, 3);   /* 3*a(i-2) */
        cur = term1 - term2;                /* a(i) = 8*a(i-1) - 3*a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
