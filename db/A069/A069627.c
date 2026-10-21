/*
 * A069627  Sum_{k=1..n} floor(n*(n-1)/(2*k)).
 * https://oeis.org/A069627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n*(n-1)/(2*k))
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    /* Compute C = n*(n-1)/2 safely in seq_t. */
    seq_t C = (seq_t)n * (seq_t)(n > 0 ? n - 1 : 0) / 2;
    for (idx_t k = 1; k <= n; ++k)
        total += C / k;   /* floor division in C */
    return total;
}
