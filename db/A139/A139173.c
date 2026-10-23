/*
 * A139173  a(n) = n!/3 - 1.
 * https://oeis.org/A139173
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / 3 - 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute n!/3 by skipping the factor 3, avoiding overflow for all
   values that fit in seq_t (works for 64‑bit and arbitrary‑precision builds). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute n! / 3 by multiplying all integers 1..n except 3.
       For n < 3 the loop handles correctly and the result matches the
       arithmetic definition (though the offset is 3). */
    seq_t prod = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        if (i == 3)          /* factor 3 is removed before division */
            continue;
        prod *= (seq_t)i;     /* cast to seq_t to avoid implicit narrowing */
    }
    /* n! / 3 is now prod, then subtract 1 as per definition */
    prod -= 1;
    return prod;
}
