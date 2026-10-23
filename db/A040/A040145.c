/*
 * A040145  Primes p such that x^4 = 19 has a solution mod p.
 * https://oeis.org/A040145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes p such that x^4 = 19 has a solution modulo p.
   a(n) is the n-th such prime.  The test for a prime p (p≠19) is
   19^((p-1)/gcd(p-1,4)) ≡ 1 (mod p).  Prime 19 is included because
   19 ≡ 0 (mod 19) and x^4 ≡ 0 has a solution. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

#define CACHE_CAP 200000   /* large enough for n up to about 1e5 */

static seq_t prime_cache[CACHE_CAP];
static size_t prime_cache_cnt = 0;
static uint64_t last_checked      = 1;   /* last prime examined */

seq_t a(idx_t n)
{
    /* Generate primes until we have at least n qualifying primes. */
    while (prime_cache_cnt < n) {
        /* Find next prime > last_checked */
        uint64_t p = last_checked + 1;
        while (!is_prime(p))
            ++p;
        last_checked = p;

        bool ok = false;
        if (p == 19)                      /* 19 divides 19 */
            ok = true;
        else if (p == 2)                  /* 2 is special: 19 ≡ 1 (mod 2) */
            ok = true;
        else {
            uint64_t g = p - 1;
            uint64_t div;
            if (g % 4 == 0)
                div = 4;
            else if (g % 2 == 0)
                div = 2;
            else
                div = 1;                 /* should not happen for odd primes */
            uint64_t exp = (p - 1) / div;
            seq_t res = mod_pow((seq_t)19, exp, (seq_t)p);
            ok = (res == (seq_t)1);
        }

        if (ok) {
            if (prime_cache_cnt < CACHE_CAP)
                prime_cache[prime_cache_cnt++] = (seq_t)p;
            else
                /* Cache overflow: should not happen for normal use. */
                return (seq_t)0;
        }
    }
    return prime_cache[n - 1];
}
