/*
 * A090085  Smallest odd pseudoprimes to base n exceeding n (like A007535 but
 *          with smallest odd terms instead of few even ones).
 * https://oeis.org/A090085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd composite m > n such that n^(m−1) ≡ 1 (mod
 *            m)
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    for (idx_t cand = n + 1; ; ++cand) {
        if ((cand & 1ULL) == 0) continue;                 /* skip even numbers */
        if (is_prime((uint64_t)cand)) continue;           /* skip primes */
        if (mod_pow(base, cand - 1, (seq_t)cand) == (seq_t)1)
            return (seq_t)cand;                          /* found the pseudoprime */
    }
}
