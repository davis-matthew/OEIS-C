/*
 * A167792  Numbers with primitive root 3.
 * https://oeis.org/A167792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A167792:  numbers m such that 3 is a primitive root modulo m.
   The function enumerates successive integers, tests each for the
   primitive‑root property using prime factorisation and modular
   exponentiation, and caches the found terms.  It works for any
   seq_t (unsigned integer or arbitrary‑precision class) and any
   idx_t (uint64_t).  All operations are on native types or seq_t. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/mod_pow.h"     /* mod_pow */

/* Check whether 3 is a primitive root modulo an odd prime p. */
static bool is_primitive_root_mod_prime(uint64_t p)
{
    if (p <= 3) return false;          /* 3 not coprime to 2 or 3 */

    seq_t mod  = (seq_t)p;
    seq_t phi  = (seq_t)(p - 1);

    /* Factor p-1.  The number of distinct primes needed is < 32. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(p - 1, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t exp = phi / (seq_t)primes[i];
        if (mod_pow((seq_t)3, (uint64_t)exp, mod) == (seq_t)1)
            return false;
    }
    return true;
}

/* Determine if m has 3 as a primitive root.
   According to known results m must be 2, 4, p^k or 2·p^k with p odd prime
   and 3 primitive root modulo p. */
static bool is_primitive_root_3(uint64_t m)
{
    if (m == 2 || m == 4) return true;

    /* Factor m. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* Case: m = p^e (odd prime power). */
    if (cnt == 1) {
        if (primes[0] == 2) return false;   /* 2^e with e>1 not allowed */
        return is_primitive_root_mod_prime(primes[0]);
    }

    /* Case: m = 2·p^e (odd prime power). */
    if (cnt == 2 && primes[0] == 2 && exps[0] == 1) {
        return is_primitive_root_mod_prime(primes[1]);
    }

    return false;  /* All other composites are rejected. */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap   = 0;
    static uint64_t next = 2;   /* next integer to test */

    if (!cache) {                  /* first call: allocate cache */
        cache_cap = 16;
        cache_size = 0;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
    }

    while (cache_size < n) {
        if (is_primitive_root_3(next)) {
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_size++] = (seq_t)next;
        }
        ++next;
    }

    return cache[n - 1];
}
