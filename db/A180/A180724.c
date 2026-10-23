/*
 * A180724  a(n) = n^2 + largest prime < n^2.
 * https://oeis.org/A180724
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + largest prime < n^2
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 262, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* undefined for n < 2, but returns 0 */
    seq_t sq = (seq_t)n * (seq_t)n; /* compute n^2 in seq_t */
    seq_t p  = sq - 1;              /* start at one less than n^2 */

    /* Find the largest prime strictly less than n^2 */
    while (!is_prime_big(p))
    {
        p = p - 1;
    }

    return sq + p;                  /* result: n^2 + largest prime < n^2 */
}
