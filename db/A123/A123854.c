/*
 * A123854  Denominators in an asymptotic expansion for the cubic recurrence
 *          sequence A123851.
 * https://oeis.org/A123854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = 2^(3*n - A000120(n))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

static inline idx_t popcount_idx(idx_t v)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(v);
#else
    idx_t c = 0;
    while (v)
    {
        v &= v - 1;
        ++c;
    }
    return c;
#endif
}

seq_t a(idx_t n)
{
    idx_t pop = popcount_idx(n);
    idx_t exp = 3 * n - pop;
    return pow2(exp);
}
