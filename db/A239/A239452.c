/*
 * A239452  Smallest integer m > 1 such that m^n == m (mod n).
 * https://oeis.org/A239452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest m > 1 such that m^n == m (mod n) */
#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* n == 1 is a special case */
    if (n == 1)
        return (seq_t)2;

    /* m ranges from 2 to n inclusive
       (OEIS note: for n>1, a(n) <= n) */
    for (uint64_t m = 2; m <= (uint64_t)n; ++m)
    {
        seq_t mod_result = mod_pow((seq_t)m, (uint64_t)n, (seq_t)n);
        if (mod_result == (seq_t)m)
            return (seq_t)m;
    }

    /* Should never reach here, but return n as a fallback */
    return (seq_t)n;
}
