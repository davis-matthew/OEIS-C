/*
 * A100089  a(n) = (3*n+1)!.
 * https://oeis.org/A100089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n+1)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute (3*n+1)! by multiplicative accumulation. */
    idx_t limit = 3 * n + 1;     /* final argument to factorial */
    seq_t result = 1;            /* factorial accumulator */

    /* Multiply result by each integer from 2 up to limit. */
    for (idx_t i = 2; i <= limit; ++i)
    {
        result *= (seq_t)i;
    }

    return result;
}
