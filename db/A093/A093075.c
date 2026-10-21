/*
 * A093075  Indicating the neighbor with a greater greatest prime factor.
 * https://oeis.org/A093075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = argmax_{s∈{-1,0,1}} gpf(n+s) subject to gpf(n+s) ≥ gpf(n)
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   _lpf_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where gpf(x) is the largest prime factor of x (defined as 1 for x ≤ 1). */

#include <stdint.h>
#include "../utils/_lpf_u64.h"

seq_t a(idx_t n)
{
    /* Largest prime factor of the current index. */
    seq_t g_n = _lpf_u64((uint64_t)n);

    seq_t bestShift = 0;
    uint64_t bestGpf   = 0;   /* will be set on first valid candidate */

    for (int shift = -1; shift <= 1; ++shift) {
        int64_t cand_i = (int64_t)n + shift;
        if (cand_i <= 0)
            continue;   /* candidates must be ≥ 1 for A006530 */

        uint64_t cand = (uint64_t)cand_i;
        seq_t g_cand  = _lpf_u64(cand);
        if (g_cand < g_n)
            continue;   /* does not satisfy the inequality */

        if (bestGpf == 0 ||
            (uint64_t)g_cand > bestGpf ||
            ((uint64_t)g_cand == bestGpf && shift > bestShift)) {
            bestShift = (seq_t)shift;
            bestGpf   = (uint64_t)g_cand;
        }
    }

    return bestShift;
}
