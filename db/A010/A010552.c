/*
 * A010552  Multiply successively by 1 (once), 2 (twice), 3 (thrice), etc.
 * https://oeis.org/A010552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(n)=a(n-1)*floor(sqrt(2*n)+1/2) for n>0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 28, u128 n <= 49,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Computes the n-th term of OEIS A010552 (Multiply successively by 1 (once), 2 (twice), 3 (thrice), etc.) */
seq_t a(idx_t n)
{
    seq_t result = 1;               /* a(0) */
    seq_t i, r, t, t_next, diff1, diff2, factor;

    for (i = 1; i <= n; ++i) {
        /* r = 2*i as seq_t to avoid overflow of idx_t multiplication */
        r = (seq_t)2 * (seq_t)i;

        /* floor sqrt of r */
        t = floor_sqrt(r);

        /* candidate factor: round(sqrt(2*i)) = floor(sqrt(2*i)+1/2) */
        t_next = t + 1;
        diff1 = r - t * t;                     /* 2*i - t^2 >= 0          */
        diff2 = t_next * t_next - r;          /* (t+1)^2 - 2*i >= 0      */
        factor = (diff1 <= diff2) ? t : t_next;

        result = result * factor;
    }

    return result;
}
