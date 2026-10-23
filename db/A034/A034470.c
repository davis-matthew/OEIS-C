/*
 * A034470  Prime numbers using only the curved digits 0, 2, 3, 5, 6, 8 and 9.
 * https://oeis.org/A034470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 849, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   allowed_digit, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A034470:  
   primes composed solely of the curved digits 0,2,3,5,6,8,9.  */
/* Uses a simple incremental search, caching results for arbitrary order
   of calls. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */
#include "../utils/allowed_digit.h"

/* static cache of already found terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static idx_t candidate = 2;          /* next number to test */

/* enlarge cache if needed */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap << 1 : 128;
    while (newcap < needed) newcap <<= 1;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1 */
    if (n == 0) return 0;   /* undefined, but harmless */

    while (cache_size < n) {
        /* search for the next admissible prime */
        for (;; ++candidate) {
            if (candidate < 2) continue;

            /* check that all decimal digits are allowed */
            idx_t t = candidate;
            bool ok = true;
            while (t > 0) {
                int d = t % 10;
                if (!allowed_digit(d)) { ok = false; break; }
                t /= 10;
            }
            if (!ok) continue;

            /* test primality */
            if (is_prime(candidate)) {
                if (cache_size == cache_cap) ensure_capacity(cache_size + 1);
                cache[cache_size++] = (seq_t)candidate;
                ++candidate;   /* move past the found prime */
                break;         /* go back to outer while to collect more */
            }
        }
    }
    return cache[n - 1];
}
