/*
 * A052750  a(n) = (2*n + 1)^(n - 1).
 * https://oeis.org/A052750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n + 1)^(n-1) for n ≥ 1, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   pow_seq_loop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_seq_loop.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    /* compute base = 2*n + 1, all in seq_t */
    seq_t base = (seq_t)2 * (seq_t)n + (seq_t)1;
    idx_t exp = n - 1u;
    return pow_seq_loop(base, exp);
}
