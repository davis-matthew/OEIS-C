/*
 * A180094  Number of steps to reach 0 or 1, starting with n and applying the
 *          map k -> (number of 1's in binary expansion of k) repeatedly.
 * https://oeis.org/A180094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of steps to reach 0 or 1 by repeatedly replacing k with
   the number of 1's in its binary expansion.  The sequence starts with
   a(0)=0, a(1)=0.  For n ≥ 2:
   a(n) = 1 + a(popcount(n))   where popcount(n) ≤ 64.
   The implementation is O(log n) per call. */
#include <stddef.h>   /* for size_t (not used) */

static inline idx_t popcount_u64(idx_t x)
{
    return __builtin_popcountll(x);
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t steps = 0;
    idx_t k = n;
    while (k > 1)
    {
        k = popcount_u64(k);
        ++steps;
    }
    return steps;
}
