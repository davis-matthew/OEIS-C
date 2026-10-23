/*
 * A161399  a(n) = the smallest divisor of n that contains the same number of
 *          (non-leading) 0's as n when both numbers are written in binary.
 * https://oeis.org/A161399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest divisor d of n such that
 *     zeros(d) == zeros(n)
 * where zeros(x) = (number of bits of x) – (number of 1-bits of x).
 * For n=1 this gives a(1)=1.
 */

#include <stdint.h>

/* count the number of zeros in the binary representation of v
   (non‑leading zeros, i.e. total bits – popcount) */
static inline int zero_count(idx_t v)
{
    /* v is guaranteed > 0 */
    int bitlen = 64 - __builtin_clzll((unsigned long long)v);
    int popcnt = __builtin_popcountll((unsigned long long)v);
    return bitlen - popcnt;
}

seq_t a(idx_t n)
{
    if (n == 0) /* safety, though n>=1 by OEIS convention */
        return (seq_t)0;

    int target_zeros = zero_count(n);
    seq_t best = (seq_t)n;        /* n itself is always a divisor */

    /* iterate over divisors up to sqrt(n) */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* candidate divisor d */
            if (zero_count(d) == target_zeros && d < (idx_t)best)
                best = (seq_t)d;

            /* paired divisor n/d */
            idx_t e = n / d;
            if (e != d && zero_count(e) == target_zeros && e < (idx_t)best)
                best = (seq_t)e;
        }
    }

    return best;
}
