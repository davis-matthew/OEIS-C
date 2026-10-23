/*
 * A108294  a(n) = the least prime p such that p-6n-3 is a power of 2 and
 *          2p-6n-3 is prime, or -1 if no such prime exists.
 * https://oeis.org/A108294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the least prime p such that p-6n-3 is a power of two
   and 2p-6n-3 is also prime.  Return -1 if no such prime exists. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    uint64_t n6 = (uint64_t)n * 6ULL;   /* 6n  (fits in 64 bits for all n used) */
    uint64_t pow2 = 1ULL;                /* current power of two 2^k */

    while (true) {
        uint64_t p = pow2 + n6 + 3ULL;   /* candidate prime 2^k + 6n + 3 */

        if (is_prime(p)) {
            /* 2p - 6n - 3 = 2^(k+1) + 6n + 3 must also be prime */
            uint64_t q = (p << 1) - n6 - 3ULL;
            if (is_prime(q))
                return (seq_t)p;
        }

        /* Stop if the next shift would overflow 64‑bit */
        if (pow2 > (UINT64_MAX >> 1))
            break;

        pow2 <<= 1;   /* next power of two 2^(k+1) */
    }

    return (seq_t)-1;   /* representation of -1 in unsigned types */
}
