/*
 * A155159  a(n) = 1 + 2*n*n!.
 * https://oeis.org/A155159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2*n*n!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Compute n! iteratively */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;          /* multiply by the next integer */

    /* Compute 1 + 2*n*n! */
    seq_t result = fact;
    result *= (seq_t)n;            /* n*n! */
    result *= 2;                   /* 2*n*n! */
    result += 1;                   /* +1 */

    return result;
}
