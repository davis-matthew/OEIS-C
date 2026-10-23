/*
 * A102515  a(n) = floor(1 + sqrt(2n + 1)).
 * https://oeis.org/A102515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(1 + sqrt(2*n + 1))
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   isqrt_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_local.h"

/* Sequence A102515 */
seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 2 + 1;      /* 2n + 1  */
    seq_t r = isqrt_local(m);        /* floor(sqrt(2n + 1)) */
    return r + 1;                    /* floor(1 + sqrt(2n + 1)) */
}
