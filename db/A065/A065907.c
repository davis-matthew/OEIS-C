/*
 * A065907  First solution mod p of x^4 = 2 for primes p such that only two
 *          solutions exist.
 * https://oeis.org/A065907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive x such that x^4 ≡ 2 (mod p), where p is
 *            the n-th prime with p ≡ 7 (mod 8)
 * Range:     u8 n <= 14, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds a list of primes up to a
   sufficient bound, selects the n-th prime ≡ 7 (mod 8),
   and uses modular exponentiation to obtain a quartic root
   of 2.  Because p ≡ 7 (mod 8) ⇒ p ≡ 3 (mod 4), the formula
   for a square root a^{(p+1)/4} works for both the intermediate
   square root of 2 and the final fourth root. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"

static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t prime_cache_limit = 0;

static void build_cache(idx_t limit)
{
    free(prime_cache);
    prime_cache_limit = limit;
    build_primes_upto(limit, &prime_cache, &prime_cache_cnt);
}

static void ensure_cache(idx_t needed)
{
    /* Ensure the cache contains enough primes (overall).  The required
       primes of the desired residue class may be found during iteration. */
    if (prime_cache_limit >= needed)
        return;
    idx_t new_limit = prime_cache_limit ? prime_cache_limit * 2 : 100;
    if (new_limit < needed) new_limit = needed;
    build_cache(new_limit);
}

seq_t a(idx_t n)
{
    /* Rough bound: about 5 * n primes cover the n-th prime ≡ 7 (mod 8) */
    idx_t estimate = n * 50 + 100;
    if (estimate < 100) estimate = 100;

    ensure_cache(estimate);

    /* Find the n-th prime congruent to 7 mod 8 */
    idx_t count = 0;
    uint64_t selected_prime = 0;

    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        if (prime_cache[i] % 8U == 7U) {
            ++count;
            if (count == n) {
                selected_prime = prime_cache[i];
                break;
            }
        }
    }

    /* If not enough, extend the cache and repeat */
    while (count < n) {
        ensure_cache(prime_cache_limit * 2);
        count = 0;
        for (size_t i = 0; i < prime_cache_cnt; ++i) {
            if (prime_cache[i] % 8U == 7U) {
                ++count;
                if (count == n) {
                    selected_prime = prime_cache[i];
                    break;
                }
            }
        }
    }

    seq_t p = (seq_t)selected_prime;
    idx_t exp = (selected_prime + 1) / 4;      /* exponent for sqrt and 4‑root */

    /* First square root of 2: s = 2^((p+1)/4) mod p   (p ≡ 3 mod 4) */
    seq_t s = mod_pow((seq_t)2, exp, p);

    /* Fourth root: t = s^((p+1)/4) mod p   (t^4 ≡ 2) */
    seq_t t = mod_pow(s, exp, p);

    /* Return the smaller representative among t and p‑t */
    seq_t p_minus_t = p - t;
    return t < p_minus_t ? t : p_minus_t;
}
