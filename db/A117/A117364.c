/*
 * A117364  a(n) = largest prime less than the largest prime dividing n (or 1
 *          if there is no such prime).
 * https://oeis.org/A117364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime less than the largest prime dividing n, or
 *            1 if there is no such prime
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gpf, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"           /* gpf(uint64_t) */
#include "../utils/prev_prime.h"    /* prev_prime(idx_t) */

seq_t a(idx_t n)
{
    if (n <= 1)                      /* 1 has no prime factors */
        return 1;

    uint64_t p = gpf((uint64_t)n);   /* largest prime factor of n */
    if (p <= 2)                      /* no smaller prime exists */
        return 1;

    idx_t prev = prev_prime((idx_t)p);   /* largest prime < p */
    if (prev < 2)
        return 1;
    return (seq_t)prev;
}
