/*
 * A033922  Base-2 digital convolution sequence.
 * https://oeis.org/A033922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=1}^{A000120(n)} a(A048793(n,k)-1), with a(0)=1 and
 *            for n>0, a(n)= Σ_{i} a(k_i) where n = Σ_{i} 2^{k_i} is the
 *            binary expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Utilities                                                     */
/* ------------------------------------------------------------------ */

/* Return floor(log2(v)) for v > 0.  v is uint64_t (idx_t). */
static inline idx_t floor_log2_uint64(idx_t v)
{
    /* __builtin_clzll counts leading zeroes in a 64‑bit value. */
    return 63 - __builtin_clzll(v);
}

/* ------------------------------------------------------------------ */
/* Global memoisation state                                      */
/* ------------------------------------------------------------------ */

static seq_t *cache = NULL;     /* cache[i] = a(i) for 0 <= i < cache_cap */
static idx_t  last_computed = 0;/* highest index whose value is known */
static size_t cache_cap     = 0; /* allocated capacity for cache       */

/* ------------------------------------------------------------------ */
/* Ensure that cache can hold values up to index n (inclusive)      */
/* ------------------------------------------------------------------ */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n < cache_cap)          /* enough room already           */
        return;

    /* Need to grow the array.  New capacity is at least n+1 (for 0..n). */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* out of memory, fatal failure */
    cache = tmp;
    cache_cap = new_cap;

    /* If this is the first allocation, initialise the base case. */
    if (cache_cap == 1) {
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        last_computed = 0;
    }
}

/* ------------------------------------------------------------------ */
/* Public API                                                     */
/* ------------------------------------------------------------------ */

/* Compute a(n) for the OEIS Base‑2 digital convolution sequence. */
seq_t a(idx_t n)
{
    ensure_cache_size(n);

    /* Value already computed? */
    if (n <= last_computed)
        return cache[n];

    /* Compute all missing values up to n, using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t rem = i;
        while (rem) {
            /* Isolate the lowest set bit of rem. */
            idx_t lsb = rem & -(idx_t)rem;
            /* Position of that bit (0‑based). */
            idx_t pos = floor_log2_uint64(lsb);
            /* Add the pre‑computed a(pos) to the running total. */
            sum += cache[pos];
            /* Remove that bit and continue. */
            rem ^= lsb;
        }
        cache[i] = sum;
    }
    last_computed = n;
    return cache[n];
}
