/*
 * A171879  a(n) = a(n-1) + a(n-2)*a(n-3) + a(n-4)*a(n-5)^a(n-6); initial
 *          terms are 0, 0, 1, 1, 1, 1.
 * https://oeis.org/A171879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 13, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2)*a(n-3)
 *                + a(n-4) * (a(n-5) ^ a(n-6))
 * for n ≥ 6.
 * Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=1.
 *   (0^0 is treated as 1)
 *
 * This implementation memoises the sequence in a dynamically resized
 * array.  It works for any seq_t (fixed‑size integer or arbitrary‑precision)
 * and any idx_t (here uint64_t).  The array expands as needed; all
 * indices up to n are guaranteed to exist after a call.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

static seq_t *cache = NULL;   /* Cache for computed terms */
static size_t cache_cap = 0;   /* Allocated capacity */
static size_t cache_size = 0;  /* Number of terms currently stored */

/* Ensure cache has capacity for indices 0 .. n */
static void ensure_cache(idx_t n) {
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed * 2;
    if (new_cap < needed)   /* guard against overflow */
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();             /* allocation failure */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Return a(n) using memoised dynamic programming */
seq_t a(idx_t n) {
    /* Make sure we can store indices up to n and up to the first
     * six initial terms.  This also lazily allocates the array.
     */
    ensure_cache(5);          /* guarantee capacity for a(0)..a(5) */
    ensure_cache(n);          /* guarantee capacity up to n       */

    /* Initialise the first six known terms lazily. */
    if (cache_size < 6) {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 1;
        cache[5] = 1;
        cache_size = 6;
    }

    /* If the requested term is already computed, return it. */
    if (n < cache_size)
        return cache[n];

    /* Compute terms from cache_size up to n. */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t term1 = cache[i-1];

        seq_t part2_left  = cache[i-2];
        seq_t part2_right = cache[i-3];
        seq_t term2 = part2_left * part2_right;

        seq_t part3_left = cache[i-4];
        seq_t base = cache[i-5];
        seq_t exp  = cache[i-6];
        seq_t pow_term;
        if (base == 0 && exp == 0)
            pow_term = (seq_t)1;              /* 0^0 = 1 */
        else
            pow_term = big_pow(base, exp);
        seq_t term3 = part3_left * pow_term;

        cache[i] = term1 + term2 + term3;
    }

    cache_size = (size_t)n + 1;   /* we now have 0 .. n */
    return cache[n];
}
