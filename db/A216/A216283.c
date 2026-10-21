/*
 * A216283  Number of nonnegative solutions to the equation x^2+5*y^2 = n.
 * https://oeis.org/A216283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions (x,y) to x^2 +
 *            5*y^2 = n
 * Range:     u8 n <= 63, u16 n <= 8055, u32 n <= 8055, u64 n <= 8055,
 *            u128 n <= 8055, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard against n=0 */

    seq_t count = 0;
    seq_t n_seq = (seq_t)n;

    /* maximum possible y: floor(sqrt(n/5)) */
    idx_t y_max = (idx_t)floor_sqrt(n_seq / 5);

    for (idx_t y = 0; y <= y_max; ++y) {
        seq_t prod = (seq_t)5 * (seq_t)y * (seq_t)y; /* 5*y^2 */
        seq_t rem  = n_seq - prod;
        seq_t s    = floor_sqrt(rem);
        if (s * s == rem) {
            count += 1;
        }
    }
    return count;
}
