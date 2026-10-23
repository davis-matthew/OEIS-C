/*
 * A019358  Primes with primitive root 32.
 * https://oeis.org/A019358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes with primitive root 32
   because 32 is a primitive root modulo a prime p iff
   for every prime divisor q of p‑1  we have 32^((p‑1)/q) ≠ 1 (mod p). */

/* build system supplies seq_t and idx_t: seq_t may be a big‑int class in C++,
   idx_t is unsigned 64‑bit.  The implementation keeps a static cache of
   such primes in a uint64_t array, converting to seq_t only when returning. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */
#include "../utils/factor_u64.h" /* void factor_u64(uint64_t,n,...,cnt) */
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base, seq_t exp, seq_t mod) */

static idx_t last_checked = 2;     /* number already examined */
static uint64_t *prime_cache = NULL;
static size_t cache_cap   = 0;
static size_t cache_cnt   = 0;

/* enlarge the cache if necessary */
static void
add_to_cache(uint64_t p)
{
    if (cache_cnt >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                            new_cap * sizeof(uint64_t));
        if (!tmp) abort();           /* allocation failure */
        prime_cache = tmp;
        cache_cap   = new_cap;
    }
    prime_cache[cache_cnt++] = p;
}

/* test whether a prime p has 32 as a primitive root */
static bool
has_primitive_root_32(idx_t p)
{
    if (p < 3) return false;      /* 2 is not valid */

    uint64_t phi = (uint64_t)p - 1;            /* φ(p) */
    uint64_t primes[64], exps[64];
    size_t prime_cnt = 0;

    factor_u64(phi, primes, exps, &prime_cnt);   /* distinct prime factors */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q   = primes[i];
        uint64_t exp = phi / q;
        seq_t r = mod_pow((seq_t)32, exp, (seq_t)p);
        if ((long long)r == 1) return false;     /* order divides (p-1)/q */
    }
    return true;
}

/* public function implementing a(n) */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset 1, but return 0 for safety */

    /* generate needed primes */
    while (cache_cnt < (size_t)n) {
        ++last_checked;
        if (!is_prime(last_checked)) continue;
        if (has_primitive_root_32(last_checked))
            add_to_cache(last_checked);
    }

    /* convert stored uint64_t to seq_t for return */
    return (seq_t)prime_cache[(size_t)n - 1];
}
