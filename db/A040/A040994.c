/*
 * A040994  Primes p such that x^6 = 3 has a solution mod p.
 * https://oeis.org/A040994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 975 (2026-11-03)
 * Helpers:   gcd, is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that the congruence x^6 ≡ 3 (mod p) has a solution */

/* The sequence is computed by testing primes in increasing order and
   keeping those for which 3 is a sixth‑power residue modulo p.
   The test for a prime p>3 is:
          g = gcd(6, p-1)
          if 3^((p-1)/g) ≡ 1 (mod p) then p is accepted.
   For p=2 or 3 the equation trivially has a solution. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for 0, return 0 to avoid UB */

    static seq_t *cache = NULL;   /* cache of discovered primes */
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static uint64_t next_val = 2; /* next integer to test for primality */

    /* Grow the cache if we need more terms */
    while (cache_size < (size_t)n) {
        /* Ensure the cache buffer is big enough */
        ensure_cache(n, &cache, &cache_cap);

        uint64_t p = next_val;
        ++next_val;

        if (!is_prime(p)) continue;   /* skip composites */

        bool ok = false;
        if (p == 2 || p == 3) {
            ok = true;                 /* 2 and 3 always satisfy the congruence */
        } else {
            seq_t g = gcd((seq_t)6, (seq_t)(p - 1));
            /* g fits in seq_t, but we need it as a divisor of (p-1) */
            uint64_t div = (uint64_t)g;
            uint64_t exp = (p - 1) / div;
            seq_t val = powmod((seq_t)3, exp, (seq_t)p);
            ok = (val == (seq_t)1);
        }

        if (ok) {
            cache[cache_size++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
