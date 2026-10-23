/*
 * A110357  a(n) is the least nonnegative integer of the form n*(n+k)/(n-k)
 *          with k>0.
 * https://oeis.org/A110357
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{k>0} n*(n+k)/(n-k) = 2*n^2/d - n * where d = max{ 1
 *            ≤ d < n | d | 2*(n-d)^2 }. * The number d is the largest divisor
 *            of 2*(n-d)^2 that is < n. * The algorithm tests d from n-1
 *            downwards, using modulo arithmetic * to avoid overflow; the
 *            first d that satisfies 2*(n-d)^2 ≡ 0 (mod d) * gi...
 * Range:     u8 n <= 11, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_mod.h"   /* mul_mod(uint64_t a, uint64_t b, uint64_t mod) */

static idx_t largest_d(idx_t n)
{
    for (idx_t d = n - 1; d > 0; --d)
    {
        idx_t val = n - d;                   /* k = val */
        uint64_t val_mod = (uint64_t)(val % d);
        /* compute (val^2 * 2) mod d without overflow           */
        uint64_t t = mul_mod(val_mod, val_mod, (uint64_t)d); /* val^2 mod d */
        t      = mul_mod(t, 2ULL, (uint64_t)d);              /* 2*val^2 mod d */
        if (t == 0)
            return d;
    }
    return 1;   /* should not happen for n >= 2 */
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* offset is 2; behaviour below is irrelevant */

    idx_t d = largest_d(n);

    /* a = 2*n^2/d - n */
    seq_t numerator = (seq_t)2 * (seq_t)n * (seq_t)n;
    seq_t quotient  = numerator / (seq_t)d;
    seq_t result    = quotient - (seq_t)n;
    return result;
}
