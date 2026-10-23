/*
 * A021318  Decimal expansion of 1/314.
 * https://oeis.org/A021318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* OEIS A021318: Decimal expansion of 1/314
 * a(0) = 0
 * a(n) = a(n-1) - a(n-39) + a(n-40)  for n > 40
 */

static seq_t *cache = NULL;      /* cache of computed digits */
static size_t len   = 0;         /* number of terms currently stored */
static size_t cap   = 0;         /* allocated capacity of cache */

/* Number of terms to pre‑compute by long division. */
#define INIT_N   41   /* compute a(0) … a(40) */

/* Denominator for the decimal expansion. */
#define DEN 314ULL

/* Ensure the cache can hold at least 'needed+1' elements. */
static void ensure_cache(idx_t needed)
{
    size_t required = (size_t)needed + 1;
    if (required <= cap) return;

    if (cap == 0) {
        cap = required < 64 ? 64 : required * 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
    } else {
        size_t newcap = cap * 2;
        if (required > newcap) newcap = required;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }
}

/* Compute a(0)…a(40) by ordinary long division of 1/314. */
static void initialize(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    ensure_cache((idx_t)(INIT_N - 1));
    seq_t rem = 1;   /* initial numerator */

    for (idx_t i = 0; i < (idx_t)INIT_N; ++i) {
        seq_t digit = (rem * 10) / (seq_t)DEN;
        cache[(size_t)i] = digit;
        rem         = (rem * 10) % (seq_t)DEN;
    }
    len = INIT_N;
}

seq_t a(idx_t n)
{
    /* Make sure that the initial long division has been performed. */
    if (len == 0) initialize();

    /* Return a cached value if available. */
    if ((idx_t)len > n) return cache[(size_t)n];

    /* Extend the cache up to index n using the recurrence. */
    while ((idx_t)len <= n) {
        /* Make sure we have space for the new element. */
        ensure_cache((idx_t)len);

        /* a(n) = a(n-1) - a(n-39) + a(n-40) for n > 40 */
        seq_t term1 = cache[(size_t)(len - 1)];
        seq_t term2 = cache[(size_t)(len - 39)];
        seq_t term3 = cache[(size_t)(len - 40)];
        seq_t val   = term1 - term2 + term3;   /* result will be 0..9 */

        cache[(size_t)len] = val;
        ++len;
    }

    return cache[(size_t)n];
}
