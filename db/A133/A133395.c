/*
 * A133395  Terms in A061725 that are of form 3*prime.
 * https://oeis.org/A133395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * q where q is prime and q = (p^2+2)/3 for some prime p
 *            ≠ 3. The function generates primes p incrementally, checks the
 *            condition, and builds the sequence on demand
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 25, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   append_candidate, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_cnt = 0;

static uint64_t next_candidate = 2;        /* next integer to test for primality */

/* Return the nth term of A133395 (1‑based index).  */
seq_t a(idx_t n)
{
    /* Ensure cache size. */
    while (cache_cnt < (size_t)n) {
        /* Find next prime p. */
        uint64_t p;
        while (1) {
            if (is_prime(next_candidate)) {
                p = next_candidate;
                next_candidate++;          /* advance for next call */
                if (p == 3)                /* skip p=3 (p^2+2 not divisible by 3) */
                    continue;
                /* Compute (p^2+2)/3 and test primality of the quotient. */
                seq_t v   = ((seq_t)p) * ((seq_t)p) + 2;
                seq_t q   = v / 3;          /* exact division because 3 | (p^2+2) */
                if (is_prime_big(q)) {
                    seq_t val = q * 3;      /* 3*q is the sequence term */
                    append_candidate(&cache, &cache_cnt, &cache_cap, val);
                    break;
                }
            }
            next_candidate++;              /* not prime or fails property */
        }
    }
    return cache[(size_t)n - 1];
}
