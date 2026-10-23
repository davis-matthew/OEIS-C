/*
 * A108541  Lesser prime factor of n-th golden semiprime.
 * https://oeis.org/A108541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1306, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Keyword: golden semiprime smaller prime factor.
   The n-th term a(n) is the smaller prime factor p of the n-th
   golden semiprime p*q, where q is the larger prime satisfying
   |phi p – q| < 1.  Equivalently, q is the nearest prime to phi p
   that lies strictly above p.  The sequence consists of all primes
   p for which either floor(phi p) or ceil(phi p) is prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static const long double phi_ld = (1.0L + sqrtl(5.0L)) / 2.0L;

/* Miller‑Rabin deterministic test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if ((n & 1u) == 0) return false;
    uint64_t d = n - 1, s = 0;
    while ((d & 1u) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        __uint128_t x = 1, p = a % n, tmp = d;
        while (tmp) {
            if (tmp & 1) x = (x * p) % n;
            p = (p * p) % n;
            tmp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) {
                comp = false;
                break;
            }
        }
        if (comp) return false;
    }
    return true;
}

/* The main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n<1, but return 0 */

    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static uint64_t next_prime_candidate = 2;  /* next prime to test */

    /* Grow cache as needed */
    while (cache_size < n) {
        /* Find the next prime p */
        uint64_t p;
        for (;;) {
            p = next_prime_candidate;
            if (p == 2 || is_prime_uint64(p)) break;
            next_prime_candidate += (p == 2) ? 1 : 2;
        }

        /* Compute candidates for the larger prime q */
        long double val = phi_ld * (long double)p;
        uint64_t q1 = (uint64_t)val;               /* floor(phi * p)   */
        bool found = false;
        if (q1 > p && is_prime_uint64(q1)) {
            found = true;
        } else {
            uint64_t q2 = q1 + 1;                 /* ceil(phi * p)    */
            if (q2 > p && is_prime_uint64(q2)) {
                found = true;
            }
        }

        /* Store p if it yields a golden semiprime */
        if (found) {
            if (cache_size + 1 > cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failure, abort */ return 0; }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)p;
        }

        /* Advance candidate for next prime */
        next_prime_candidate += (p == 2) ? 1 : 2;
    }
    return cache[n - 1];
}
