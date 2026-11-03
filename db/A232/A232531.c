/*
 * A232531  Numbers n such that the equation a^2 + 2*n*b^2 = 2*c^2 + n*d^2 has
 *          no solutions in positive integers for a, b, c, d.
 * https://oeis.org/A232531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8300 (2026-11-01)
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A232531
   a(n) = the nth positive integer that has at least one prime factor
         p ≡ 3 (mod 8) or p ≡ 5 (mod 8) with an odd exponent. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

/* Return true if m has a prime divisor p ≡ 3 or 5 (mod 8) occurring
   an odd number of times in the prime factorization of m. */
static bool has_odd_bad_prime(uint64_t m, const idx_t *primes, size_t cnt)
{
    while (m > 1) {
        uint64_t p = 0;
        /* find the smallest prime factor of m */
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t pr = primes[i];
            if (pr * pr > m) break;
            if (m % pr == 0) { p = pr; break; }
        }
        if (p == 0) p = m;          /* m itself is prime */

        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        uint64_t mod8 = p & 7ULL;    /* p % 8 */
        if ( (mod8 == 3ULL || mod8 == 5ULL) && (exp & 1) ) return true;
    }
    return false;
}

static idx_t *prime_list = NULL;
static size_t prime_cnt  = 0;
static bool  primes_ready = false;

seq_t a(idx_t n)
{
    if (!primes_ready) {
        /* build a list of primes sufficient for factoring numbers
           up to a few hundred thousand (well beyond the 10 000‑th term). */
        build_primes(200000, &prime_list, &prime_cnt);
        primes_ready = true;
    }

    uint64_t target = (uint64_t)n;
    if (target == 0) return (seq_t)0;  /* undefined but safe */
    uint64_t m       = 1;              /* start from 1 */
    uint64_t found   = 0;

    for (;;) {
        ++m;
        if (has_odd_bad_prime(m, prime_list, prime_cnt)) {
            ++found;
            if (found == target) return (seq_t)m;
        }
    }
}
