/*
 * A151784  a(n) = 6^(wt(n) - 1) where wt(n) = A000120(n).
 * https://oeis.org/A151784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^(wt(n)-1) where wt(n) = A000120(n) (popcount of n)
 * Range:     u8 n <= 30, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the number of 1-bits in n.  n ≥ 1, so the
       result is at least 1. */
    unsigned int pop = __builtin_popcountll((unsigned long long)n);

    /* Exponent: wt(n) - 1.  The cast to idx_t is safe because
       pop ≤ 64. */
    idx_t exp = (idx_t)pop - 1;

    seq_t result = 1;
    const seq_t base = 6;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;

    return result;
}
