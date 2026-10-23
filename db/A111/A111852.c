/*
 * A111852  Number of numbers m <= n such that 2 equals the first digit after
 *          decimal point of square root of n in decimal representation.
 * https://oeis.org/A111852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2621, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Count m ≤ n such that the first digit after the decimal
   point of √m is 2.
   For each m we compute
       k = ⌊√m⌋
       t = ⌊10·√m⌋ = ⌊√(100·m)⌋
   then diff = t – 10·k = ⌊10·(√m – k)⌋.
   If diff == 2 we have a qualifying number. */
#include <stdint.h>
#include "../utils/int_sqrt.h"  /* declares idx_t int_sqrt(idx_t); */

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t k  = int_sqrt(i);
        idx_t t  = int_sqrt((idx_t)(((__uint128_t)i * 100)));
        idx_t diff = t - (10 * k);
        if (diff == 2) {
            ++cnt;
        }
    }
    return cnt;
}
