/*
 * A178949  E.g.f. satisfies: A(x) = exp(x^2*A(x)) where A(x) = Sum_{n>=0}
 *          a(n)*x^(2n)/(2n)!.
 * https://oeis.org/A178949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^(n-1) * (2n)! / n! for n ≥ 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 88 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_u64_idx.h"

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
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute prod = Π_{k=n+1}^{2n} k = (2n)! / n! */
    seq_t prod = (seq_t)1;
    for (idx_t k = n + 1; k <= (idx_t)(2 * n); ++k) {
        prod = mul_small(prod, (unsigned int)k);
    }

    /* Compute pw = (n+1)^(n-1) */
    seq_t pw = ipow_u64_idx((uint64_t)(n + 1), n - 1);

    /* Final result = prod * pw */
    return prod * pw;
}
