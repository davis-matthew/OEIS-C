/*
 * A090098  Bases such that the smallest prime-power-pseudoprime is belonging
 *          to equals 25.
 * https://oeis.org/A090098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 6117, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A090098: Bases such that the smallest prime‑power
   pseudoprime equals 25.  The algorithm tests each integer b
   for the failure of pseudoprimality at m = 4, 8, 9, 16 and the
   success at m = 25.  The first n such integers are cached. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Test whether b is a prime‑power pseudoprime to base m (m a prime power). */
static inline bool pp_pseudoprime(seq_t b, uint64_t m)
{
    /* gcd must be 1 for pseudoprime; otherwise it fails automatically */
    if (gcd(b, (seq_t)m) != (seq_t)1)
        return false;

    uint64_t mod_b = (uint64_t)(b % (seq_t)m);
    uint64_t exp = m - 1;
    uint64_t res = 1 % m;   /* 1 mod m */

    while (exp) {
        if (exp & 1) res = (res * mod_b) % m;
        mod_b = (mod_b * mod_b) % m;
        exp >>= 1;
    }
    return res == (1 % m);
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_base = 1;

    while (cache_len < (size_t)n) {
        seq_t base = (seq_t)next_base;
        bool ok = true;

        /* prime powers less than 25 must NOT be pseudoprimes */
        const uint64_t fails[] = {4, 8, 9, 16};
        for (size_t i = 0; i < sizeof(fails)/sizeof(fails[0]); ++i) {
            if (pp_pseudoprime(base, fails[i])) {
                ok = false;
                break;
            }
        }

        /* 25 must be a pseudoprime */
        if (ok && !pp_pseudoprime(base, 25))
            ok = false;

        if (ok) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = base;
        }
        ++next_base;
    }

    return cache[(size_t)(n - 1)];
}
