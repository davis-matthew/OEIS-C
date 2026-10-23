/*
 * A024547  a(n) = [ n/{n*sqrt(3)} ], where {x} := x - [ x ].
 * https://oeis.org/A024547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n / { n*sqrt(3) } )
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 211, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Compute a(n) using only integer arithmetic.
 * The derivation uses the inequality
 *
 *   a D - n k   <=  sqrt(3)*n^2
 *  (a+1)D - n k  >  sqrt(3)*n^2
 *
 * where k = floor( sqrt(3)*n ) = floor( sqrt(3*n^2) ),
 *        D = 3 n^2 - k^2  (positive).
 * Squaring eliminates sqrt(3) and yields only integer comparisons.
 */
seq_t a(idx_t n)
{
    /* Safety for the offset 1. a(0) is undefined but return 0. */
    if (n == 0)
        return 0;

    seq_t sn = (seq_t)n;                 /* n as seq_t */
    seq_t sqn = sn * sn;                 /* n^2 */

    /* k = floor(sqrt(3*n^2)) */
    seq_t tri = (seq_t)3 * sqn;          /* 3*n^2 */
    seq_t k   = floor_sqrt(tri);

    /* D = 3*n^2 - k^2 */
    seq_t d = tri - k * k;

    /* target = 3 * n^4 = 3 * (n^2)^2 */
    seq_t n4 = sqn * sqn;
    seq_t target = (seq_t)3 * n4;

    /* Binary search for the largest a such that (a*d - n*k)^2 <= target */
    idx_t low  = 0;
    idx_t high = (idx_t)3 * n * n;        /* 3*n^2 is a safe upper bound */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;            /* candidate a */
        seq_t mid_s = (seq_t)mid;
        seq_t prod  = mid_s * d;                     /* mid*d */
        seq_t left  = prod - sn * k;                 /* a*d - n*k */
        seq_t left2 = left * left;                   /* square for comparison */

        if (left2 <= target)
            low = mid;
        else
            high = mid - 1;
    }

    /* 'low' now holds the desired a(n) */
    return (seq_t)low;
}
