/*
 * A138966  a(n) = n + (smallest composite > n).
 * https://oeis.org/A138966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n + smallest composite > n
 *
 * This implementation searches strictly upward from n+1 until it finds the
 * first integer that is greater than 3 and not prime.  That integer is the
 * smallest composite greater than n, and its sum with n gives the sequence
 * value.  The search uses the utility function is_prime(idx_t) provided by
 * the build environment.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1 */

    idx_t cand = n + 1;                 /* start searching from n+1 */
    /* Skip 2 and 3 – they are not composites. */
    while (true)
    {
        if (cand <= 3) { cand++; continue; }
        if (!is_prime(cand)) break;     /* found the smallest composite > n */
        cand++;
    }

    /* Compute the result; cast to seq_t as required. */
    return (seq_t)n + (seq_t)cand;
}
