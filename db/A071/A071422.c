/*
 * A071422  a(n) = a(n-1) + sum of decimal digits of sigma(n), the sum of
 *          divisors of n.
 * https://oeis.org/A071422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = a(n-1) + σ₁(n)₀₁₂₃₄₅₆₇₈₉₍sum of decimal digits of σ(n)),
 *            a(1) = 1
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute σ(n) (sum of divisors) by a simple sieve and accumulate the
 *  sequence values.  The function is memoised so that multiple calls
 *  with arbitrary n are handled efficiently.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: digit sum for 64‑bit numbers (σ(n) never exceeds 2·n << 10). */
static inline uint64_t digit_sum_uint64(uint64_t x)
{
    uint64_t sum = 0;
    while (x) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* Static cache for previously computed values */
static idx_t cache_limit = 0;         /* largest n computed so far        */
static seq_t *a_cache      = NULL;    /* a[1..cache_limit]                */
static uint64_t *sigma     = NULL;    /* sigma[1..cache_limit]            */

/* Resize and recompute the cache up to at least n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_limit) return;

    /* allocate new storage */
    size_t new_size = (size_t)(n + 1);      /* we use 1‑based indices   */
    if (sigma == NULL) {
        sigma = (uint64_t*)calloc(new_size, sizeof(uint64_t));
    } else {
        sigma = (uint64_t*)realloc(sigma, new_size * sizeof(uint64_t));
        if (sigma) memset(sigma + cache_limit + 1, 0, (new_size - cache_limit - 1) * sizeof(uint64_t));
    }
    if (a_cache == NULL) {
        a_cache = (seq_t*)calloc(new_size, sizeof(seq_t));
    } else {
        a_cache = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
    }

    /* compute divisor sums up to n by a sieve */
    for (size_t i = 1; i <= n; ++i)
        for (size_t j = i; j <= n; j += i)
            sigma[j] += (uint64_t)i;

    /* compute a[n] values */
    if (cache_limit == 0) {            /* initialise a[1] */
        a_cache[1] = (seq_t)1;
        cache_limit = 1;
    }
    for (size_t i = cache_limit + 1; i <= n; ++i) {
        seq_t prev = a_cache[(size_t)i - 1];
        uint64_t ds = digit_sum_uint64(sigma[i]);
        a_cache[i] = prev + (seq_t)ds;
    }
    cache_limit = n;
}

/* Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* under‑offset behaviour is unspecified */
    ensure_cache(n);
    return a_cache[(size_t)n];
}
