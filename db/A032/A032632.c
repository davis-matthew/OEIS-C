/*
 * A032632  Primes that are decimal concatenations of n with n + 9.
 * https://oeis.org/A032632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime obtained by concatenating an integer k
 *            with k+9 (i.e. seq_t concat_parts(k, k+9) is prime)
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 2317, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 48 terms through n = 2025 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Static cache of found primes */
static seq_t  *cache      = NULL;   /* array of primes */
static idx_t  cache_cap  = 0;      /* allocated capacity */
static idx_t  cache_len  = 0;      /* number of primes stored */
static idx_t  next_k     = 1;      /* next k to test */

/* Ensure cache can hold at least 'need' elements.  Memory is allocated
   with `alloc_seq_array`; old memory is left unreclaimed (small leak is
   harmless for the problem size). */
static void ensure_cache_cap(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache = alloc_seq_array(new_cap);
    if (cache) {
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                  /* undefined below offset */

    /* Return cached value if already computed. */
    if (n <= cache_len) return cache[n - 1];

    /* Compute terms until we reach index n. */
    while (cache_len < n) {
        while (1) {
            seq_t cand = concat_parts(next_k, next_k + 9);
            if (isPrimeGeneric(cand)) {
                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = cand;
                ++next_k;                      /* advance to next k */
                break;
            }
            ++next_k;
        }
    }
    return cache[n - 1];
}
