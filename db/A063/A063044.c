/*
 * A063044  Collatz-2 (A063041) trajectory starting at 29.
 * https://oeis.org/A063044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A063044: Collatz‑2 (A063041) trajectory starting at 29.
   a(0) = 29
   a(n) = A063041(a(n‑1)) for n > 0
   The recurrence:
        if x even:  x/2
        if x odd and prime:  x * (previous prime < x) + 1
        if x odd composite:  x / (smallest prime factor of x)                   */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Forward declarations of helper functions that operate on seq_t */
static bool is_prime_small(seq_t v);
static seq_t prev_prime(seq_t v);
static seq_t next_value(seq_t v);

/* Cache for memoisation */
static seq_t *cache = NULL;      /* array of computed terms */
static idx_t cache_len = 0;      /* number of terms stored (next free index) */
static idx_t cache_cap = 0;      /* capacity of cache */

/* Ensure that the cache can hold at least 'need' elements */
static void ensure_cache_capacity(idx_t need)
{
    if (cache_cap >= need)
        return;
    if (cache_cap == 0)
        cache_cap = 1;
    while (cache_cap < need)
        cache_cap *= 2U;
    seq_t *new_cache = (seq_t *)realloc(cache,
                                         (size_t)cache_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* allocation failure */
    cache = new_cache;
}

/* Return the joint function A063041 for one step of the sequence */
static seq_t next_value(seq_t v)
{
    /* even case: divide by 2 */
    if ((v & 1U) == 0U)
        return v / 2U;

    /* odd case: test for primality */
    if (is_prime_small(v)) {
        seq_t p = prev_prime(v);           /* previous prime < v */
        return v * p + 1U;                 /* v*prev_prime + 1 */
    }

    /* odd composite: divide by its smallest prime factor */
    seq_t fac = 0U;
    for (seq_t f = 3U; f <= v / f; f += 2U) {
        if (v % f == 0U) {
            fac = f;
            break;
        }
    }
    /* v is composite, fac must be non‑zero */
    return v / fac;
}

/* Simple trial‑division primality test for seq_t values */
static bool is_prime_small(seq_t v)
{
    if (v < 2U)
        return false;
    if (v == 2U)
        return true;
    if ((v & 1U) == 0U)
        return false;
    for (seq_t i = 3U; i <= v / i; i += 2U) {
        if (v % i == 0U)
            return false;
    }
    return true;
}

/* Find the largest prime less than v (assumes v is an odd prime ≥ 3) */
static seq_t prev_prime(seq_t v)
{
    if (v <= 3U)
        return 2U;
    seq_t k = v - 1U;
    for (;;) {
        if ((k & 1U) == 0U)
            k -= 1U;                    /* skip even numbers */
        if (is_prime_small(k))
            return k;
        if (k <= 3U)
            return 2U;
        k -= 2U;                        /* next candidate */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure cache can hold at least n+1 terms */
    ensure_cache_capacity((size_t)n + 1U);

    /* Initialise cache with a(0) if empty */
    if (cache_len == 0U) {
        cache[0] = (seq_t)29U;   /* a(0) = 29 */
        cache_len = 1U;
    }

    /* Compute missing terms up to index n */
    while (cache_len <= n) {
        seq_t prev = cache[cache_len - 1U];
        seq_t curr = next_value(prev);
        cache[cache_len++] = curr;
    }

    return cache[n];
}
