/*
 * A133828  a(n) = the smallest "isolated divisor" of n, or 0 if no such
 *          divisor exists. A positive divisor, k, of n is isolated if neither
 *          (k-1) nor (k+1) divides n.
 * https://oeis.org/A133828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 513, u16 n <= 7772, u32 n <= 7772, u64 n <= 7772,
 *            u128 n <= 7772, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest isolated divisor of n.
   For odd n the answer is 1; for even n it iterates over the
   divisors of n to find the first divisor k>1 such that neither
   k-1 nor k+1 divides n.  Returns 0 if no such divisor exists. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Work with unsigned 64‑bit arithmetic for safety */
    uint64_t N = (uint64_t)n;

    /* a(1) = 1, and all odd n give 1 */
    if (N == 1u || (N & 1u) != 0u)
        return (seq_t)1u;

    /* n = 2 has no isolated divisor */
    if (N == 2u)
        return (seq_t)0u;

    /* Search for the smallest isolated divisor >1.
       Start at 3 because 2 can never be isolated (1 always divides n). */
    for (uint64_t d = 3u; d <= N; ++d)
    {
        if (N % d != 0u)          /* d is not a divisor of n */
            continue;

        if (N % (d - 1u) == 0u)   /* (d‑1) divides n -> not isolated */
            continue;

        uint64_t next = d + 1u;
        if (next != 0u && N % next == 0u) /* (d+1) divides n -> not isolated */
            continue;

        return (seq_t)d;          /* smallest isolated divisor found */
    }

    /* No isolated divisor >1 was found */
    return (seq_t)0u;
}
