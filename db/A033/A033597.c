/*
 * A033597  (nextprime(n)+prevprime(n))/2.
 * https://oeis.org/A033597
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (nextprime(n)+prevprime(n))/2
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prev_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 4: a(4) is the first term given. */
    if (n < 4)
        return 0;   /* value undefined below the offset */

    /* prev_prime(n) returns the largest prime < n. */
    idx_t pprev = prev_prime(n);

    /* next_prime(start) returns the smallest prime > start. */
    uint64_t pnext = next_prime((uint64_t)n);

    seq_t sprev = (seq_t)pprev;
    seq_t snext = (seq_t)pnext;

    return (sprev + snext) / 2;
}
