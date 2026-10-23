/*
 * A194920  a(n) = n - floor(n/sqrt(2)).
 * https://oeis.org/A194920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor(n / sqrt(2))
 * Range:     u8 n <= 15, u16 n <= 401, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_n_over_sqrt2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_n_over_sqrt2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    return (seq_t)n - floor_n_over_sqrt2(n);
}
