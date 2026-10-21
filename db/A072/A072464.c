/*
 * A072464  Code word lengths for non-redundant MML code for positive
 *          integers.
 * https://oeis.org/A072464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(⌊log₂ n⌋) + ⌊log₂ n⌋ + 1 (with a(1)=1) Equivalently,
 *            a(n) = Σ_{i≥0} (⌊log₂^i(n)⌋ + 1) where log₂^0(n) = n and
 *            log₂^{i+1}(x) = ⌊log₂(x)⌋. This gives the lengths of the
 *            non‑redundant MML code for positive integers (the universal code
 *            of an Elias omega coding)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

static inline idx_t floor_log2_u64(idx_t x) {
    /* x > 0 */
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll(x);
#else
    /* Portable fallback */
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
#endif
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t cur = n;
    while (cur > 0) {
        idx_t k = floor_log2_u64(cur);
        sum += (seq_t)k + 1;
        cur = k;
    }
    return sum;
}
