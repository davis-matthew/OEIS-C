/*
 * A137923  Zerofree numbers k such that A061486(k) is prime.
 * https://oeis.org/A137923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 5049, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9891 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A137923: zerofree integers k for which A061486(k) is prime.
   A061486(k) is the sum of products of all non‑empty contiguous
   subsequences of the decimal digits of k.
   Returns the nth such integer (1‑based). */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* compute A061486(n) (sum of products of all non‑empty contiguous
 * subsequences of decimal digits). Returns the value as `seq_t`
 * if it fits; otherwise returns a value greater than UINT64_MAX
 * to indicate overflow (treated as non‑prime). */
static seq_t a061486(uint64_t n)
{
    /* gather digits in reverse order (least significant first) */
    unsigned char digits[32];
    size_t len = 0;
    uint64_t t = n;
    while (t > 0) {
        digits[len++] = t % 10;
        t /= 10;
    }
    /* compute sum of products of all contiguous substrings */
    unsigned __int128 sum128 = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned __int128 prod = 1;
        for (size_t j = i; j < len; ++j) {
            prod *= digits[j];
            sum128 += prod;
        }
    }
    /* check overflow relative to maximum value representable in seq_t */
    unsigned __int128 max_seq = (unsigned __int128)(seq_t)-1;
    if (sum128 > max_seq) return (seq_t)0; /* treat as overflow */
    return (seq_t)sum128;
}

/* cache of found terms */
static seq_t *cache   = NULL;   /* array of seq_t */
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity of cache */

/* candidate number to test next (starting after 1) */
static uint64_t candidate = 1;

/* enlarge cache if needed */
static void maybe_grow_cache(void)
{
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
}

/* check for zero digit */
static bool has_zero_digit(uint64_t n)
{
    while (n > 0) {
        if ((n % 10) == 0) return true;
        n /= 10;
    }
    return false;
}

/* test whether candidate is in the sequence */
static bool candidate_prime(uint64_t cand)
{
    if (has_zero_digit(cand)) return false;
    seq_t val = a061486(cand);
    if (val == (seq_t)0) return false;      /* overflow -> not a prime */
    return isPrimeGeneric(val);
}

/* ensure cache contains at least n terms */
static void ensure_cache(idx_t n)
{
    while (cache_size < (size_t)n) {
        bool found = false;
        while (!found) {
            ++candidate;                     /* start after the last tested */
            if (has_zero_digit(candidate))
                continue;
            if (candidate_prime(candidate)) {
                maybe_grow_cache();
                cache[cache_size++] = (seq_t)candidate;
                found = true;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */
    ensure_cache(n);
    return cache[cache_size - 1];
}
