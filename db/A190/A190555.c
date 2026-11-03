/*
 * A190555  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(2),4,2) and []=floor.
 * https://oeis.org/A190555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4n+2)⋅sqrt(2)) – 4⋅floor(n⋅sqrt(2)) – 2
 * Range:     u8 none, u16 n <= 22, u32 n <= 128, u64 n <= 128, u128 n <= 128,
 *            big any n
 * Validated: partial, pass, 128 terms through n = 128 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where n ≥ 1 (offset 1).                                    */
/* Uses integer arithmetic only and the provided floor_sqrt.  */

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

/* external linkages as required */
seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1) */
    seq_t nsq = (seq_t)n * (seq_t)n;                      /* n²          */
    seq_t nb   = floor_sqrt((seq_t)2 * nsq);              /* floor(n√2)  */

    /* 4n + 2 */
    seq_t t = (seq_t)4 * (seq_t)n + (seq_t)2;
    seq_t tsq = t * t;                                    /* (4n+2)²     */
    seq_t nt   = floor_sqrt((seq_t)2 * tsq);              /* floor((4n+2)√2) */

    /* a(n) = nt - 4*nb - 2   (all operations fit in seq_t)   */
    return nt - ((seq_t)4 * nb) - (seq_t)2;
}
