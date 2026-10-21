/*
 * A237626  Sum of a^2 + b^2 for all nonnegative integers a,b such that
 *          b^2-a^2 = 4n.
 * https://oeis.org/A237626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|4n, d≤d', same parity} (d²+d'²)/2, where d·d' = 4n,
 *            so that a = (d'−d)/2, b = (d'+d)/2
 * Range:     u8 n <= 7, u16 n <= 154, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t val   = 0;
    seq_t fourn = (seq_t)n * 4;                /* 4n as seq_t   */

    /* integer square‑root of 4n (fast path for all builds)  */
    seq_t sqrt_flo = floor_sqrt(fourn);        /* floor(sqrt(4n)) */
    idx_t limit = (idx_t)sqrt_flo;             /* safe cast, sqrt(4n) ≤ 2³³ for n<2⁶⁴ */

    for (idx_t d = 1; d <= limit; ++d)
    {
        seq_t sd  = (seq_t)d;                  /* divisor as seq_t */

        /* check if d divides 4n */
        if ((fourn % sd) != 0)
            continue;

        seq_t dprime = fourn / sd;             /* complementary divisor */

        /* both must have same parity for integer a,b          */
        if (((sd ^ dprime) & 1) != 0)
            continue;

        seq_t sumSq = sd * sd + dprime * dprime; /* d² + d'²  */
        val += sumSq / 2;                       /* (d²+d'²)/2 */
    }

    return val;
}
