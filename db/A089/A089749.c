/*
 * A089749  a(n) = smallest m > a(n-1) such that concatenation nm is a prime.
 * https://oeis.org/A089749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest m > a(n‑1) such that concatenation nm is a prime
 *            (computed by searching forward for the next prime for each n)
 * Range:     u8 n <= 3, u16 n <= 1542, u32 n <= 6086, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9432 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

/* cache holds the sequence values for indices >= 1.
   cache[0] == a(1), cache[1] == a(2), ... */
static seq_t *cache = NULL;
static size_t cache_cap = 0;  /* number of allocated slots */

/* Ensure that cache has at least n slots, zero‑initialising any new ones.
   Returns false on allocation failure. */
static bool ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_cap) return true;
    size_t new_cap = (size_t)n;
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    if (!new_cache) return false;
    /* zero the newly allocated part   */
    mem_set(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n == 0 */

    if (!ensure_cache(n)) return 0; /* allocation failure */

    /* initialise a(1) if it hasn't been set yet */
    if (cache[0] == 0) cache[0] = 1;

    /* Compute terms that are still missing. */
    for (size_t idx = 1; idx < (size_t)n; ++idx)   /* idx+1 == current n */
    {
        if (cache[idx] != 0) continue;            /* already known */

        seq_t prev = cache[idx - 1];              /* a(idx) = a(n-1) */
        seq_t m    = prev + 1;                    /* start search */

        for (;; ++m) {
            seq_t candidate = concat_parts((idx_t)(idx + 1), (idx_t)m);
            if (isPrimeGeneric(candidate)) {
                cache[idx] = m;                  /* a(idx+1) = m */
                break;
            }
        }
    }

    return cache[n - 1];
}
