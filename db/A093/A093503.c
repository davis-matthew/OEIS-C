/*
 * A093503  a(0) = 2, a(n) = least prime >= a(n-1) + n.
 * https://oeis.org/A093503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 2; a(n) = least prime >= a(n‑1) + n
 * Range:     u8 n <= 19, u16 n <= 354, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is very small for n ≤ 10 000, so we can compute it
       naively without any caching or memoisation.  The running time
       is dominated by the (quite cheap) primality tests for numbers
       up to about 5×10⁷, and a few hundred thousand such tests still
       finish well under two seconds.  The algorithm is completely
       deterministic and works for every build.  It uses only
       integer arithmetic; all intermediate values are 64‑bit
       quantities that are safely cast to seq_t when returned. */

    uint64_t value = 2;                    /* a(0) */
    if (n == 0)
        return (seq_t)value;

    for (idx_t i = 1; i <= n; ++i) {
        uint64_t candidate = value + (uint64_t)i;   /* a(n‑1)+n */
        /* Find the next prime ≥ candidate.  The helper is_prime()
           works for the 64‑bit range of idx_t. */
        while (!is_prime(candidate))
            ++candidate;
        value = candidate;
    }

    return (seq_t)value;
}
