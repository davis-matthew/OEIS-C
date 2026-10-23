/*
 * A147762  a(n) is the smallest positive integer m with exactly n zeros and
 *          exactly n ones in its binary representation and with n represented
 *          in binary as a substring of the binary representation of m.
 * https://oeis.org/A147762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 2^(2n-1) + 2^(n-1) - 1 if n is a power of two, else a(n)
 *            = 2^(2n-1) + (n+1)·2^m - 1 where m = n-1 - popcount(n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/mul_small.h"

/* count 1-bits in n (popcount) */
static unsigned int popcount_u64(uint64_t v)
{
    unsigned int c = 0;
    while (v)
    {
        c += (unsigned int)(v & 1);
        v >>= 1;
    }
    return c;
}

/* check whether n is a power of two */
static bool is_power_of_two_u64(uint64_t v)
{
    return v != 0 && (v & (v - 1)) == 0;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1 by the specification of the sequence */
    idx_t rev = n;  /* local copy */

    /* compute 2^(2n-1) */
    seq_t part = pow2(rev * 2 - 1);

    if (is_power_of_two_u64(rev))
    {
        /* a(n) = 2^(2n-1) + 2^(n-1) - 1 */
        part += pow2(rev - 1);
        part -= 1;
    }
    else
    {
        /* m = n - 1 - popcount(n) */
        idx_t m = rev - 1 - (idx_t)popcount_u64(rev);
        seq_t pow_m = pow2(m);
        /* (n+1)·2^m */
        seq_t mult = mul_small(pow_m, (unsigned)(rev + 1));
        part += mult;
        part -= 1;
    }

    return part;
}
