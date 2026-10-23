/*
 * A180497  a(n)=n^2-3*floor[n/sqrt(3)]^2.
 * https://oeis.org/A180497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2-3*floor[n/sqrt(3)]^2
 * Range:     u8 n <= 13, u16 n <= 169, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include <stdbool.h>

/* Compute a(n) as defined in OEIS A180497 */
seq_t a(idx_t n)
{
    /* According to the offset, n >= 1.  For n < 1 we simply return 0. */
    if (n == 0) return 0;

    /* Work entirely in integer arithmetic. */
    seq_t sq   = (seq_t)n * (seq_t)n;            /* n^2  */
    seq_t div  = sq / 3;                         /* floor(n^2 / 3)  */
    seq_t k    = floor_sqrt(div);                /* k = floor(n/√3) */
    seq_t res  = sq - 3 * k * k;                 /* a(n) = n^2 - 3k^2   */
    return res;
}
