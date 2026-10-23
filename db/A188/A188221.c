/*
 * A188221  a(n) = [n*r + k*r]-[n*r]-[k*r], where r=sqrt(5), k=1, [ ]=floor.
 * https://oeis.org/A188221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n*sqrt(5)+sqrt(5)] - [n*sqrt(5)] - [sqrt(5)] which
 *            simplifies to a(n) = floor((n+1)*sqrt(5)) - floor(n*sqrt(5)) - 2
 *            producing 0 or 1 as in the Sturmian sequence with slope
 *            sqrt(5)-2
 * Range:     u8 n <= 2, u16 n <= 56, u32 n <= 148, u64 n <= 148,
 *            u128 n <= 148, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n) {
    /* Ensure n >= 1; behavior below offset is irrelevant. */
    seq_t nn = (seq_t)n;             /* n as seq_t */
    seq_t t1 = nn * nn;              /* n^2   */
    t1 = t1 * 5;                     /* 5*n^2 */
    seq_t cur = floor_sqrt(t1);      /* floor(n*sqrt(5)) */

    seq_t n1 = nn + (seq_t)1;        /* n+1 */
    seq_t t2 = n1 * n1;              /* (n+1)^2 */
    t2 = t2 * 5;                     /* 5*(n+1)^2 */
    seq_t next = floor_sqrt(t2);     /* floor((n+1)*sqrt(5)) */

    seq_t diff = next - cur;         /* either 2 or 3 */
    return diff - 2;                 /* 0 for diff==2, 1 for diff==3 */
}
