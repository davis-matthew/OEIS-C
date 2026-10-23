/*
 * A133150  a(n) = smallest k such that A000290(n+1) = A000290(n) +
 *          (A000290(n) mod k), or 0 if no such k exists.
 * https://oeis.org/A133150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133150: a(n) = smallest k such that (n+1)^2 = n^2 + (n^2 mod k);
   a(n) = 0 if no such k exists.  Uses the divisor property:
   n^2 % k = 2n+1 ⇔ k | (n^2 - 2n - 1) and k > 2n+1.
   Therefore a(n) is the smallest divisor of m = n^2 - 2n - 1 that exceeds 2n+1. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Handle trivial cases where n^2 < 2n+1 (no solution) */
    if (n == 0) return 0;               // offset safety
    seq_t n_sq   = (seq_t)n * (seq_t)n;   /* n^2 */
    seq_t diff   = (seq_t)2 * (seq_t)n + 1; /* 2n+1 */
    if (n_sq < diff) return 0;          /* m would be negative */

    seq_t m = n_sq - diff;              /* m = n^2 - 2n - 1 */

    seq_t sqrt_m = floor_sqrt(m);
    idx_t limit = (idx_t)sqrt_m;        /* loop limit fits in idx_t for n ≲ 10⁴ */

    seq_t best = 0;                     /* 0 → no divisor found yet */

    for (idx_t d = 1; d <= limit; ++d)
    {
        seq_t div = (seq_t)d;
        if (m % div == 0)
        {
            seq_t comp = m / div;      /* complementary divisor */
            if (comp > diff)
            {
                if (best == 0 || comp < best)
                    best = comp;
            }
        }
    }
    return best;                        /* 0 if none found */
}
