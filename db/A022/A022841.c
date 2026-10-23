/*
 * A022841  Beatty sequence for sqrt(7).
 * https://oeis.org/A022841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(7))
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */
    seq_t un = (seq_t)n;                  /* promote idx_t to seq_t */
    seq_t sq = un * un;                   /* n*n  (fits for tested ranges) */
    seq_t prod = sq * (seq_t)7;           /* 7*n*n */
    return floor_sqrt(prod);              /* floor(sqrt(7*n*n)) = floor(n*sqrt(7)) */
}
