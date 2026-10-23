/*
 * A101315  Primes p such that p+2, p^2 - 2p + 2, and p^2 - 2p + 4 are all
 *          prime.
 * https://oeis.org/A101315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime p such that p+2, p²-2p+2, and p²-2p+4 are all prime.
   The function performs a simple linear search over odd integers, using
   the provided 64‑bit primality test.  The algorithm stops when the
   desired count is reached. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_uint64.h"

/* helper: test whether p satisfies the conditions */
static inline bool satisfies(uint64_t p)
{
    /* p must be odd and ≥ 3 */
    if (p < 3) return false;
    if (!is_prime_uint64(p)) return false;
    if (!is_prime_uint64(p + 2)) return false;

    /* compute q = p^2 - 2p + 2 and r = q + 2 using 128‑bit arithmetic
       to avoid overflow for moderate p.  Values that exceed 64‑bit are
       automatically rejected because they cannot be tested by the
       64‑bit is_prime_uint64 routine. */
    __uint128_t pp = p;
    __uint128_t q128 = pp * pp - 2 * pp + 2;
    __uint128_t r128 = q128 + 2;

    if (q128 > UINT64_MAX || r128 > UINT64_MAX) return false;
    uint64_t q = (uint64_t)q128;
    uint64_t r = (uint64_t)r128;

    if (!is_prime_uint64(q)) return false;
    if (!is_prime_uint64(r)) return false;
    return true;
}

/* external interface required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n = 0, return 0 */

    uint64_t count = 0;                /* how many qualifying primes so far */

    for (uint64_t p = 3; ; p += 2) {   /* iterate over odd numbers only */
        if (satisfies(p)) {
            ++count;
            if (count == n) return (seq_t)p;
        }
    }
}
