/*
 * A130252  Partial sums of A130250.
 * https://oeis.org/A130252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} ceil(log_2(3*k-1)) (with A130250(0)=0)
 * Range:     u8 n <= 42, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t y = 3 * k - 1;                     /* value inside the log */
        unsigned f = floor_log2_uint64(y);        /* floor(log_2(y))  */
        bool is_pow2 = ((y & (y - 1)) == 0);     /* y is a power of 2? */
        sum += (seq_t)(is_pow2 ? f : f + 1);      /* ceil(log_2(y))   */
    }
    return sum;
}
