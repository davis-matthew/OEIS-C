/*
 * A190555  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(2),4,2) and []=floor.
 * https://oeis.org/A190555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4n+2)⋅sqrt(2)) – 4⋅floor(n⋅sqrt(2)) – 2
 * Range:     u8 none, u16 n <= 22, u32 n <= 128, u64 n <= 128, u128 n <= 128,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where n ≥ 1 (offset 1).                                    */
/* Uses integer arithmetic only and the provided floor_sqrt.  */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

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
