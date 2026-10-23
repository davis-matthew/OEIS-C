/*
 * A147760  a(n) is the smallest positive integer m with exactly n ones in its
 *          binary representation and with n represented in binary as a
 *          substring of the binary representation of m.
 * https://oeis.org/A147760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (n+1)*2^(n - popcount(n)) - 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (equivalent to a(n) = n*2^m + 2^m - 1 where m = n - popcount(n)) */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* count number of 1 bits in n (popcount) */
    idx_t ones = __builtin_popcountll(n);

    /* m = n - popcount(n) */
    idx_t m = n - ones;

    /* 2^m as seq_t */
    seq_t power = pow2(m);

    /* (n + 1) * 2^m - 1 */
    seq_t result = ((seq_t)(n + 1)) * power - 1;

    return result;
}
