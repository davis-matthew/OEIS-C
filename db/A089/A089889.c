/*
 * A089889  Number of subsets of {1,.., n} containing exactly one square.
 * https://oeis.org/A089889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊√n⌋ * 2^(n - ⌊√n⌋) (OEIS A089889)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"   /* declares seq_pow(seq_t, idx_t) */

/* Compute floor(sqrt(n)) for non‑negative n (uint64_t). */
static uint64_t floor_sqrt_u64(uint64_t n)
{
    uint64_t s = 0;
    for (int i = 62; i >= 0; --i) {          /* shift 1ULL<<62 is safe */
        uint64_t cand = s | (1ULL << i);
        if (cand <= n / cand)
            s = cand;
    }
    return s;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; n=0 is unused */
        return (seq_t)0;

    uint64_t root_u = floor_sqrt_u64((uint64_t)n);
    seq_t root = (seq_t)root_u;                     /* ⌊√n⌋ as seq_t */
    seq_t pow  = seq_pow((seq_t)2, (idx_t)(n - root_u)); /* 2^(n-⌊√n⌋) */
    return root * pow;                              /* product, exact */
}
