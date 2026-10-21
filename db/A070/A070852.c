/*
 * A070852  Smallest prime == 1 mod (8n).
 * https://oeis.org/A070852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest prime congruent to 1 modulo 8*n (i.e. the
 *            least prime p such that p ≡ 1 (mod 8*n))
 * Range:     u8 n <= 12, u16 n <= 480, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset starts at 1; this case is unused */
    /* Compute the modulus 8*n as a 64‑bit integer.  n is uint64_t, so the
       multiplication stays in 64 bits for the expected input sizes. */
    uint64_t mod  = 8ULL * (uint64_t)n;
    /* First candidate >1 that satisfies p ≡ 1 (mod mod). */
    uint64_t p = mod + 1ULL;
    /* Scan upward in steps of mod until a prime is found. */
    while (!is_prime_uint64(p))
        p += mod;
    return (seq_t)p;
}
