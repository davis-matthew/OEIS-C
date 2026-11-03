/*
 * A186156  Rank of n^3 when {i^3: i>=1} and {2j^2: j>=1} are jointly ranked
 *          with i^3 before 2j^2 when i^3=2j^2. Complement of A186157.
 * https://oeis.org/A186156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor( floor_sqrt( 2*n^3 - 1 ) / 2 )
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 89 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Compute the nth term of OEIS A186156: rank of n^3 in the joint
 * ordering of i^3 and 2*j^2, with i^3 ranked before 2*j^2 in ties.
 */
seq_t a(idx_t n)
{
    if (n == 0)      /* offset 1 – value for n=0 is unused in the
                         sequence, but defined for safety */
        return 0;

    seq_t nn   = (seq_t)n;          /* promote index to seq_t */
    seq_t n3   = nn * nn * nn;      /* n^3 */
    seq_t t    = n3 * 2 - 1;        /* 2*n^3 - 1 */
    seq_t s    = floor_sqrt(t);     /* floor(sqrt(2*n^3-1)) */
    seq_t half = s / 2;             /* floor(sqrt(2*n^3-1)/2) */
    return nn + half;               /* final rank */
}
