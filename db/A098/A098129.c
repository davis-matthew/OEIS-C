/*
 * A098129  Concatenate 1 once, 2 twice, 3 three times, up to n n times.
 * https://oeis.org/A098129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098129: Concatenate 1 once, 2 twice, …, n n times. */
/* The value a(n) is built by repeatedly appending the decimal
 * representation of k, k times, for k = 1…n.  The implementation
 * uses an incremental construction:
 *
 *   a(0) = 0
 *   for k = 1..n:
 *       digits = number of decimal digits of k
 *       pow10_d = 10^digits
 *       for i = 1..k:
 *           a[k] = a[k-1] * pow10_d + k
 *
 * Memoisation is employed so that any call with a larger n only
 * extends the previously computed terms.  The algorithm
 * is O(n²) in the number of scalar multiplications, which is
 * sufficient for the limits of the test suite (n up to ~10000)
 * when seq_t is an arbitrary‑precision type.
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* cache[0] is dummy for a(0) */
    static size_t cache_cap = 0;     /* allocated size of cache (including index 0) */
    static idx_t max_cached = 0;     /* largest n for which cache[n] is valid */

    /* Pre‑compute small powers of ten once */
    static seq_t pow10[21];
    static int pow10_ready = 0;
    if (!pow10_ready) {
        pow10[0] = (seq_t)1;
        for (int i = 1; i < 21; ++i) pow10[i] = pow10[i-1] * (seq_t)10;
        pow10_ready = 1;
    }

    /* Ensure cache can hold up to index n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure – out of scope */
        cache = new_cache;
        /* initialise new entries with 0 (they will be overwritten) */
        for (size_t i = cache_cap; i <= n; ++i) cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* Extend the cache up to n if necessary */
    if (n > max_cached) {
        for (idx_t k = max_cached + 1; k <= n; ++k) {
            /* Previous value a(k-1) */
            seq_t prev = (k > 1) ? cache[k-1] : (seq_t)0;

            /* number of decimal digits of k */
            idx_t tmp = k;
            idx_t digits = 0;
            do { ++digits; tmp /= 10; } while (tmp);

            seq_t pow10_d = pow10[digits];  /* 10^digits */

            /* Build a(k) by appending k, k times */
            seq_t val = prev;
            for (idx_t i = 0; i < k; ++i) {
                val = val * pow10_d + (seq_t)k;
            }
            cache[k] = val;
        }
        max_cached = n;
    }

    return cache[n];
}
