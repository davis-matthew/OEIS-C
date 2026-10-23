/*
 * A189007  Number of ON cells after n generations of the 2D cellular
 *          automaton described in the comments.
 * https://oeis.org/A189007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 1 a(n) = 2^( popcount(n-1) + floor_log2(n-1) +
 *            1 ) for n > 1
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 512, u64 n <= 512,
 *            u128 n <= 512, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

static inline idx_t popcount_idx(idx_t x)
{
    idx_t c = 0;
    while (x) { c++; x &= x - 1; }
    return c;
}

static inline idx_t floor_log2_idx(idx_t x)
{
    idx_t l = 0;
    while (x >>= 1) l++;
    return l;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    idx_t k = n - 1;                /* n-1 > 0 for n > 1 */
    idx_t pop = popcount_idx(k);
    idx_t lg  = floor_log2_idx(k);
    idx_t exp = pop + lg + 1;       /* exponent of 2 */
    return pow2(exp);
}
