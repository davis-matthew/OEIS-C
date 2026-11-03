/*
 * A049547  Primes p such that x^15 = 2 has a solution mod p.
 * https://oeis.org/A049547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 969 (2026-11-01)
 * Helpers:   gcd_u64, isqrt, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime p for which the congruence
   x^15 ≡ 2 (mod p) is solvable.  For an odd prime p it is equivalent
   to 2^((p-1)/gcd(15,p-1)) ≡ 1 (mod p), and p = 2 is always a
   solution.  The function keeps a static list of all primes found so
   far and a list of qualifying primes, extending both lists on demand. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/isqrt.h"
#include "../utils/powmod.h"

/* Allocate or grow an array of uint64_t */
static inline uint64_t *grow(uint64_t *arr, size_t *cap)
{
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    uint64_t *tmp = (uint64_t *)realloc(arr, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    *cap = new_cap;
    return tmp;
}

/* Euclidean gcd for uint64_t */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static state */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt  = 0, prime_cap  = 0;
    static uint64_t *qual_list = NULL;
    static size_t qual_cnt  = 0, qual_cap  = 0;
    static uint64_t next_candidate = 2;

    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    /* Grow qualifying list initially if needed */
    if (qual_cap == 0) {
        qual_cap = 16;
        qual_list = (uint64_t *)malloc(qual_cap * sizeof(uint64_t));
        if (!qual_list) abort();
    }

    while (qual_cnt < (size_t)n) {
        /* Find the next prime */
        uint64_t p = 0;
        for (;;) {
            uint64_t cand = next_candidate++;
            if (cand < 2) continue;
            if (cand == 2) { p = 2; break; }
            if ((cand & 1) == 0) continue;   /* skip even numbers */

            /* trial division by known primes up to sqrt(cand) */
            bool is_prime = true;
            uint64_t limit = isqrt((idx_t)cand);
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t q = prime_list[i];
                if (q > limit) break;
                if (cand % q == 0) { is_prime = false; break; }
            }
            if (is_prime) { p = cand; break; }
        }

        /* Record the prime for future trial divisions */
        if (prime_cnt == prime_cap) {
            prime_list = grow(prime_list, &prime_cap);
        }
        prime_list[prime_cnt++] = p;

        /* Test the 15th‑root condition */
        bool qualifies = false;
        if (p == 2) {
            qualifies = true;
        } else {
            uint64_t m = p - 1;
            uint64_t g = gcd_u64(15, m);
            uint64_t exp = m / g;
            if (powmod((seq_t)2, exp, (seq_t)p) == (seq_t)1)
                qualifies = true;
        }

        /* Store qualifying primes */
        if (qualifies) {
            if (qual_cnt == qual_cap) {
                qual_list = grow(qual_list, &qual_cap);
            }
            qual_list[qual_cnt++] = p;
            if (qual_cnt == (size_t)n) {
                return (seq_t)p;
            }
        }
    }

    /* Should never reach here */
    return (seq_t)0;
}
