/*
 * A194152  Beatty sequence for 5+2*sqrt(5); complement of A194151.
 * https://oeis.org/A194152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(5+2*sqrt(5)))
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 7212, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    unsigned long long k = (unsigned long long)n * 2ULL;          /* k = 2n */
    unsigned long long tmp = 5ULL * k * k;                        /* tmp = 5*(2n)^2 */
    seq_t t = floor_sqrt((seq_t)tmp);                             /* t = floor(2n*sqrt(5)) */
    return (seq_t)n * 5ULL + t;                                   /* a(n) = 5*n + t */
}
