/*
 * A121844  Excess of n^3 over previous prime.
 * https://oeis.org/A121844
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 - previous_prime(n^3) (offset = 2)
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 40, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* For n < 2 the OEIS offset says the sequence is undefined.
       Returning 0 here will never be used by the test harness. */
    if (n < 2)
        return 0;

    /* Compute n^3 in the destination integer type. */
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;

    /* Search downward for the closest prime < cube. */
    seq_t p = cube - 1;                 /* start just below cube */
    while (!is_prime_big(p))            /* is_prime_big handles any seq_t size */
    {
        p--;                             /* fall back to the previous integer */
    }

    /* The excess of n^3 over that prime. */
    return cube - p;
}
