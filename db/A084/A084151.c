/*
 * A084151  Binomial transform of a Pell convolution.
 * https://oeis.org/A084151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((3+sqrt(8))^n + (3-sqrt(8))^n - 2*3^n)/16 Equivalent
 *            recurrence (for n ≥ 2): a(n) = 6*a(n-1) - a(n-2) + 3^(n-2) with
 *            a(0) = 0, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for a(n) and for powers of 3 (3^k). */
static seq_t *a_cache   = NULL;  /* stores a(0) … a(k) */
static seq_t *pow3_cache = NULL; /* stores 3^0 … 3^k        */
static idx_t cache_size = 0;      /* number of entries already computed */

/* Ensure cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Determine new required size. */
    idx_t new_size = n + 1;            /* number of elements needed */
    /* Resize arrays if necessary. */
    if (a_cache == NULL || new_size > cache_size) {
        a_cache     = (seq_t*)realloc(a_cache,     new_size * sizeof(seq_t));
        pow3_cache  = (seq_t*)realloc(pow3_cache,  new_size * sizeof(seq_t));
    }

    /* Initialisation of the first term(s). */
    if (cache_size == 0) {
        a_cache[0]    = 0;
        pow3_cache[0] = 1;                 /* 3^0 */
        cache_size    = 1;
        /* If only a(0) is required we are done. */
        if (new_size <= 1) return;
    }

    if (cache_size <= 1 && new_size > 1) {
        a_cache[1]    = 0;
        pow3_cache[1] = 3;                 /* 3^1 */
        cache_size    = 2;
    }

    /* Compute missing entries. */
    for (idx_t i = cache_size; i < new_size; ++i) {
        /* pow3_cache[i] = pow3_cache[i-1] * 3 */
        pow3_cache[i] = pow3_cache[i-1] * (seq_t)3;

        /* Recur for a(i) only when i >= 2. */
        if (i >= 2) {
            /* a(i) = 6*a(i-1) - a(i-2) + 3^(i-2) */
            a_cache[i] = 6 * a_cache[i-1] - a_cache[i-2] + pow3_cache[i-2];
        }
    }
    cache_size = new_size;
}

/* Public function required by the interface. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
