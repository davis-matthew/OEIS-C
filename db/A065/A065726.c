/*
 * A065726  Primes p whose base-8 expansion is also the decimal expansion of a
 *          prime.
 * https://oeis.org/A065726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterate primes and keep those whose
 * base‑8 digit string, read as a decimal number, is also prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"   /* not used but included for completeness */

/* ---------- local Miller–Rabin for 64‑bit integers ---------- */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static uint64_t mod_pow(uint64_t a, uint64_t d, uint64_t m)
{
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mod_mul(r, a, m);
        a = mod_mul(a, a, m);
        d >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n-1 as d * 2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64-bit integers */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- sequence algorithm ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t current_prime = 2;  /* next prime candidate */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the sequence */

    /* grow cache until we have the requested term */
    while (cache_len < (size_t)n) {
        /* test current_prime */
        if (is_prime_uint64(current_prime)) {
            /* convert to base‑8 digits (least significant first) */
            uint64_t t = current_prime;
            uint8_t digits[32];
            size_t len = 0;
            if (t == 0) {
                digits[0] = 0;
                len = 1;
            } else {
                while (t > 0) {
                    digits[len++] = (uint8_t)(t % 8);
                    t /= 8;
                }
            }

            /* interpret those digits as a decimal number */
            uint64_t dec = 0;
            for (size_t i = 0; i < len; ++i) {
                dec = dec * 10 + digits[len - 1 - i];
            }

            if (is_prime_uint64(dec)) {
                /* ensure enough space in the cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;   /* allocation failure – unlikely */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                /* only store if the prime fits in the seq_t type */
                seq_t val = (seq_t)current_prime;
                if ((uint64_t)val == current_prime) {
                    cache[cache_len++] = val;
                }
            }
        }

        /* move to the next prime candidate */
        current_prime = next_prime(current_prime);
    }

    return cache[n - 1];
}
