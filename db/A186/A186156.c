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
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

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
