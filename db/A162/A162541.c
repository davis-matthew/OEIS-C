/*
 * A162541  Primes p such that a splitting of the cyclic group Zp by the
 *          perfect 3-shift code {+-1,+-2,+-3} exists.
 * https://oeis.org/A162541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   floor_sqrt, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A162541: primes p that can be written as 7x²±6xy+36y². The test uses the
   discriminant condition Δ = 28p − 972y² and checks whether Δ is a perfect
   square with correct parity, i.e. whether (Δ¹/² ± 6y) is divisible by 14. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/floor_sqrt.h"
#include "../utils/next_prime.h"
#include "../utils/seq_realloc.h"

static bool is_special_prime(seq_t p)
{
    if (p < 7)
        return false;

    for (idx_t y = 0; ; ++y) {
        seq_t ysq   = (seq_t)y * (seq_t)y;
        seq_t term  = (seq_t)972 * ysq;          /* 972 y² */
        seq_t rem   = (seq_t)28 * p;              /* 28 p  */

        if (term > rem)                         /* 28p − 972y² < 0   */
            break;

        seq_t delta = rem - term;
        seq_t s     = floor_sqrt(delta);
        if (s * s != delta)
            continue;

        seq_t sixy   = (seq_t)6 * (seq_t)y;
        idx_t mod1   = ((idx_t)(s % (seq_t)14) + (idx_t)(sixy % (seq_t)14)) % 14;
        idx_t mod2   = ((idx_t)(s % (seq_t)14) + 14 -
                        (idx_t)(sixy % (seq_t)14)) % 14;
        if (mod1 == 0 || mod2 == 0)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n<1 */

    /* Static cache for primes found so far */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t prime_cursor = 2; /* next candidate start */

    /* Extend cache until it contains at least n elements */
    while (cache_len < n) {
        uint64_t p_uint = next_prime(prime_cursor);
        seq_t p = (seq_t)p_uint;

        if (is_special_prime(p)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp)
                    return 0; /* allocation failed – abort */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = p;
        }
        prime_cursor = p_uint + 1;
    }

    return cache[n - 1];
}
