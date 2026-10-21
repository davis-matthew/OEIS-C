/*
 * A089888  Number of subsets of {1,.., n} containing at least one square.
 * https://oeis.org/A089888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^⌊√n⌋ − 1) · 2^(n − ⌊√n⌋) (offset 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Integer square root for 64‑bit values, using binary search. */
static idx_t
isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = ((idx_t)1ULL << 32) - 1;      /* ⌊√(2^64-1)⌋ < 2^32 */

    while (lo <= hi) {
        idx_t mid   = lo + ((hi - lo) >> 1);
        idx_t mid_sq = mid * mid;
        if (mid_sq <= x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n = 0. */
    if (n == 0) return 0;

    /* k = ⌊√n⌋ */
    idx_t k = isqrt_u64(n);

    /* Compute 2^k and 2^(n−k). */
    seq_t pow_k    = big_pow((seq_t)2, (seq_t)k);
    seq_t pow_nk   = big_pow((seq_t)2, (seq_t)(n - k));

    /* (2^k − 1) · 2^(n−k). */
    return (pow_k - (seq_t)1) * pow_nk;
}
