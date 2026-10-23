/*
 * A105229  a(n) = Sum_{k=0..n} (1-(-1)^( floor( (-n-k)/2^k ) )) * 2^(k-1).
 * https://oeis.org/A105229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (1-(-1)^( floor((-n-k)/2^k) )) * 2^(k-1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build‑time types: seq_t is the integer type for sequence values,
   idx_t is an unsigned integer type for indices (typically uint64_t). */
#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle k = 0 explicitly */
    seq_t sum = 0;
    if (n != 0) {                 /* n > 0: ceil(n/1) = n */
        if ((uint64_t)n & 1ULL)   /* odd n -> contribute 2^0 == 1 */
            sum += (seq_t)1;
    }
    if (n == 0)                    /* only k = 0 contributes, which was covered above */
        return sum;

    /* Find the smallest k ≥ 1 such that 2^k > n + k.
       For all k ≥ threshold, ceil((n+k)/2^k) = 1 (odd), so all higher terms
       contribute 2^k. */
    size_t threshold = 1;
    while (threshold < 64) {        /* safe shift for 64‑bit \\\\n */
        uint64_t pow2k = ((uint64_t)1 << threshold); /* 2^threshold */
        if (pow2k > (uint64_t)n + (uint64_t)threshold)
            break;
        ++threshold;
    }

    /* Low part: 1 ≤ k < threshold */
    for (size_t k = 1; k < threshold; ++k) {
        uint64_t pow2k = ((uint64_t)1 << k);                /* 2^k, k ≤ 63 */
        /* ceil((n+k)/2^k) = (n + k + (2^k - 1)) >> k */
        uint64_t val = ((uint64_t)n + k + pow2k - 1) >> k;
        if (val & 1ULL)                        /* odd -> contribute 2^k */
            sum += pow2(k);
    }

    /* High part: k ≥ threshold and k ≤ n */
    if (threshold <= n) {
        seq_t high = pow2((uint64_t)n + 1ULL) - pow2(threshold);
        sum += high;
    }

    return sum;
}
