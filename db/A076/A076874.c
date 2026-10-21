/*
 * A076874  a(n) = n - floor ( ( 4*n + 1 )^(1/2) ).
 * https://oeis.org/A076874
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - floor(sqrt(4*n + 1))
 * Range:     u8 n <= 63, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt_seq.h"

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;   /* offset is 3, values below are irrelevant */
    seq_t tn = (seq_t)n;
    seq_t v = tn * 4 + 1;
    seq_t s = floor_sqrt_seq(v);
    return tn - s;
}
