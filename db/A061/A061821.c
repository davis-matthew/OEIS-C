/*
 * A061821  Multiples of 5 containing only digits 0,...,5.
 * https://oeis.org/A061821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061821: multiples of 5 containing only digits 0–5.
   This implementation iterates over multiples of 5, keeps those whose
   decimal representation uses only digits 0…5 and caches the results.
   Thus a(n) is the n-th such multiple, with a(1)=0. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache     = NULL;  /* cached terms */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_len = 0;      /* number of terms stored so far */
static seq_t last_candidate = 0;  /* next multiple of 5 to test */

/* Ensure the cache can hold at least 'needed' terms. */
static void
ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;

    /* In C++ `realloc` returns a void*, so cast explicitly. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache     = tmp;
    cache_cap = new_cap;
}

/* Check whether all decimal digits of 'x' are in the set {0,…,5}. */
static bool
digits_ok(seq_t x)
{
    if (x == 0)           /* 0 is allowed */
        return true;
    while (x > 0) {
        uint8_t d = (uint8_t)(x % 10);
        if (d > 5)
            return false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 0.  Undefined behavior for n < 1; return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len)
        return cache[n - 1U];

    /* Generate terms until the n-th is found. */
    seq_t cand = last_candidate;
    while (cache_len < (size_t)n) {
        if (digits_ok(cand)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = cand;
        }
        cand += 5;  /* next multiple of 5 */
    }

    /* Update for next call. */
    last_candidate = cand;

    return cache[n - 1U];
}
