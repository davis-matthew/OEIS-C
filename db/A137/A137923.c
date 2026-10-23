/*
 * A137923  Zerofree numbers k such that A061486(k) is prime.
 * https://oeis.org/A137923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 5049, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a061486, isPrimeGeneric, seq_realloc
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
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_realloc.h"
#include "../utils/a061486.h"

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
