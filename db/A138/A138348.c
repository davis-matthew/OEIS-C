/*
 * A138348  Lesser of twin primes such that both twin primes have no bases b,
 *          1 < b < p-1, in which p is a palindrome.
 * https://oeis.org/A138348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   floor_sqrt, grow_seq_array, is_prime_generic, next_prime,
 *            seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A138348:
 *  Lesser of twin primes p such that both p and p+2 have no palindrome representation
 *  in any base 1 < b < p-1.  Computed by incremental prime search.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/is_prime_generic.h"
#include "../utils/next_prime.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/grow_seq_array.h"

/* Helper: test if n is a palindrome in the given base (base ≥ 2). */
static bool is_palindrome_in_base(seq_t n, uint64_t base)
{
    if (base < 2)
        return true;                  /* safety for edge cases */

    seq_t  b = (seq_t)base;
    uint32_t digits[128];             /* big enough for base ≤ 2^32 and 64‑bit numbers */
    size_t  len = 0;
    seq_t x = n;

    while (x > (seq_t)0) {
        digits[len++] = (uint32_t)(x % b);
        x /= b;
    }

    for (size_t i = 0; i < len / 2; ++i) {
        if (digits[i] != digits[len - 1 - i])
            return false;
    }
    return true;
}

/* Helper: test the “no‑palindrome‑in‑any‑base” property for a prime p. */
static bool passes_base_property(seq_t p)
{
    seq_t sqrt_p = floor_sqrt(p);
    uint64_t limit = (uint64_t)sqrt_p;      /* sqrt(p) fits in 64 bits for our search range */

    for (uint64_t base = 2; base <= limit; ++base) {
        if (is_palindrome_in_base(p, base))
            return false;
    }
    return true;
}

/* Memoised sequence storage. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_len  = 0;      /* number of terms stored */
static uint64_t last_checked = 1;  /* last prime examined */

/* Ensure the cache has room for at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed)
            new_cap = needed;

        if (!cache) {
            cache = seq_alloc_array(new_cap);
            cache_cap = new_cap;
        } else {
            grow_seq_array(&cache, &cache_cap, new_cap);
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n < 1 */

    while (cache_len < (size_t)n) {
        /* Find the next prime after the last one we examined. */
        uint64_t p = next_prime((idx_t)(last_checked + 1));
        last_checked = p;

        seq_t p_big = (seq_t)p;
        seq_t pp2   = p_big + (seq_t)2;

        /* Twin‑prime test: p+2 must also be prime. */
        if (!is_prime_generic((idx_t)pp2))
            continue;

        /* Property test for both primes. */
        if (!passes_base_property(p_big))
            continue;
        if (!passes_base_property(pp2))
            continue;

        /* Store the lesser twin prime. */
        ensure_capacity(cache_len + 1);
        cache[cache_len++] = p_big;
    }

    return cache[(size_t)(n - 1)];
}
