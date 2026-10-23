/*
 * A162357  a(n) is the smallest prime p beginning with 2n such that the
 *          difference between p and the next prime is 2n.
 * https://oeis.org/A162357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: smallest prime p whose decimal representation begins with the digits of 2*n
   and for which the gap to the next prime is exactly 2*n.  This implements OEIS A162357. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

/* ---------------------------------------------------------- */
/* Helper: compute 10^d as uint64_t (d will stay small).    */
static inline uint64_t pow10_uint64(uint64_t d)
{
    uint64_t r = 1;
    for (uint64_t i = 0; i < d; ++i) r *= 10ULL;
    return r;
}

seq_t a(idx_t n)
{
    /* 2*n as the prefix that the prime must start with.           */
    uint64_t prefix = 2ULL * (uint64_t)n;   /* fits in 64 bits for n up to 10^4. */
    uint64_t pow10d = 1;                    /* 10^d  (d = 0, 1, 2, …)         */
    for (uint64_t d = 0;; ++d) {
        uint64_t low  = prefix * pow10d;     /* smallest number beginning with prefix */
        uint64_t high = (prefix + 1) * pow10d - 1;   /* largest number with same prefix */

        /* Find the first prime ≥ low, then scan all primes in this block. */
        uint64_t p = next_prime(low);
        while (p <= high) {
            uint64_t nextp = next_prime(p + 1);     /* next prime after p */
            if (nextp - p == 2ULL * n)              /* gap equals 2*n?      */
                return (seq_t)p;                    /* answer found         */
            p = next_prime(p + 1);                  /* move to the following prime in this block */
        }

        /* No suitable prime in this digit-length block: try the next (more digits). */
        pow10d *= 10ULL;   /* safe: will never overflow for the sizes we need. */
    }
}
