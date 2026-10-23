/*
 * A105131  Primes of the form 512n+257.
 * https://oeis.org/A105131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A105131: primes of the form 512n + 257
   a(n) is the nth prime p such that p ≡ 257 (mod 512). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Public function required by the framework.
   It searches candidates 512*k + 257 and counts primes
   until the nth one is found.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    idx_t found = 0;
    uint64_t k = 0;

    while (found < n) {
        uint64_t cand = 512ULL * k + 257ULL;
        if (is_prime(cand)) {
            ++found;
            if (found == n) return (seq_t)cand;
        }
        ++k;
    }

    /* This point is never reached. */
    return 0;
}
