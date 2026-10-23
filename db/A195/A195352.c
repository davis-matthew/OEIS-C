/*
 * A195352  Smallest prime p such that 2*n+1 = 2*p + q for some odd prime q.
 * https://oeis.org/A195352
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that 2*n+1 = 2*p + q for an odd
 *            prime q
 * Range:     u8 n <= 2574, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the helper is_prime(idx_t) defined in the
 * "../utils/is_prime.h" library.  No additional data structures or
 * caching are required for the range of n that the testing harness
 * exercises (up to about 10 000).  A simple linear search for the
 * smallest admissible p is fast enough. */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

/* Return the value of the sequence at index n (n >= 3).  For indices
 * below the offset the function returns 0; the value is not defined. */
seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;           /* undefined for the pre‑offset */

    /* The odd integer 2*n+1 to be represented as 2*p + q. */
    seq_t target = (seq_t)2 * (seq_t)n + 1;

    /* Search for the smallest prime p. */
    for (idx_t p = 2; /* infinite loop; break when found */; ++p)
    {
        if (!is_prime(p))
            continue;              /* p must be prime */

        seq_t q = target - (seq_t)2 * (seq_t)p;
        if (q < 3)                 /* p too large, no further candidates */
            break;

        if (is_prime((idx_t)q))
            return (seq_t)p;       /* found the smallest admissible p */
    }

    /* Under Lemoine's conjecture a representation exists for all odd
     * integers > 5, so this point should never be reached. */
    return (seq_t)0;
}
