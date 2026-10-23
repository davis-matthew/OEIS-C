/*
 * A152951  Complementary von Staudt prime numbers.
 * https://oeis.org/A152951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 870, u32 n <= 9287, u64 n <= 9287,
 *            u128 n <= 9287, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complementary von Staudt prime numbers.  A(n) is the n-th prime of
   the form 12k–1 that is NOT a von Staudt prime, i.e. 12p ≠
   denominator( B(p–1)/(p–1) ).  This function computes that value. */

#include "../utils/is_prime.h"
#include <stdlib.h>
#include <stdbool.h>

/* Return true iff p is a von Staudt prime.
   The condition is equivalent to:  the product of all primes q≠p
   for which (q–1)|(p–1) is ≤12.  The function stops early when the
   product exceeds 12. */
static bool is_von_staudt_prime(idx_t p)
{
    idx_t m = p - 1;      /* p is prime, so m is even */
    unsigned long long prod = 1;   /* product of qualifying primes */

    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d != 0) continue;

        idx_t q1 = d + 1;
        if (q1 != p && is_prime(q1)) {
            prod *= q1;
            if (prod > 12) return false;   /* not a von Staudt prime */
        }

        idx_t d2 = m / d;
        if (d2 != d) {
            idx_t q2 = d2 + 1;
            if (q2 != p && is_prime(q2)) {
                prod *= q2;
                if (prod > 12) return false;
            }
        }
    }

    /* Product ≤ 12 → p is a von Staudt prime */
    return true;
}

/* Cache for the computed sequence values. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t next_candidate = 11;   /* first number ≡11 (mod12) */

/* Grow the memory for cache if needed. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Keep cache[0..n] filled. */
static void ensure_cache_for(idx_t n)
{
    if (cache_len > n) return;

    grow_cache(cache_len + (n + 1 - cache_len));

    while (cache_len <= n) {
        /* Find next admissible prime of form 12k-1. */
        while (true) {
            if (is_prime(next_candidate) &&
                !is_von_staudt_prime(next_candidate))
                break;
            next_candidate += 12;
        }
        cache[cache_len++] = (seq_t)next_candidate;
        next_candidate += 12;
    }
}

seq_t a(idx_t n)
{
    ensure_cache_for(n);
    return cache[n];
}
