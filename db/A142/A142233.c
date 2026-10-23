/*
 * A142233  Primes congruent to 36 mod 41.
 * https://oeis.org/A142233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 160, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime that is congruent to 36 modulo 41.  */
/* a(n) = prime p with p ≡ 36 (mod 41) and it is the n‑th such prime.   */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* index type: idx_t (uint64_t)  */
/* value type: seq_t (unsigned, possibly arbitrary precision) */

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term; n == 0 is undefined. */
    if (n == 0) {
        return (seq_t)0;          /* safety return */
    }

    idx_t count = 0;

    /* Enumerate all integers of the form 36 + 41*i.  */
    for (idx_t i = 0; ; ++i) {
        idx_t candidate = 36ULL + i * 41ULL;
        if (candidate < 2) {
            continue;              /* skip 0,1, etc. */
        }
        if (is_prime(candidate)) {  /* built‑in primality test */
            ++count;
            if (count == n) {
                return (seq_t)candidate;   /* found the n‑th prime */
            }
        }
    }
}
