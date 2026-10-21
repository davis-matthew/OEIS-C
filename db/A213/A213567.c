/*
 * A213567  Principal diagonal of the convolution array A213566.
 * https://oeis.org/A213567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 3)*Fib(n+3) + (n^2 + 2)*Fib(n+2) - 4*(n^2 + 2*n +
 *            2)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 31, u64 n <= 73, u128 n <= 162,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* handle the offset: a(1) is the first term */
    if (n == 0)
        return 0; /* undefined for n<1, but guard anyway */

    seq_t sn = n;                          /* index as seq_t */
    seq_t fib_n2 = fib(n + 2);             /* Fib(n+2) */
    seq_t fib_n3 = fib(n + 3);             /* Fib(n+3) */

    /* (2*n + 3) * Fib(n+3) */
    seq_t part1 = (sn * 2 + 3) * fib_n3;

    /* (n^2 + 2) * Fib(n+2) */
    seq_t part2 = (sn * sn + 2) * fib_n2;

    /* 4*(n^2 + 2*n + 2) */
    seq_t sub_factor = sn * sn + sn * 2 + 2;
    seq_t sub = sub_factor * 4;

    /* a(n) = part1 + part2 - sub */
    return part1 + part2 - sub;
}
