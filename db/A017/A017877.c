/*
 * A017877  Expansion of 1/(1 - x^9 - x^10).
 * https://oeis.org/A017877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-9) + a(n-10) for n > 9; a(0) = 1, a(1..8) = 0, a(9) =
 *            1, a(10) = 1
 * Range:     u8 n <= 102, u16 n <= 178, u32 n <= 331, u64 n <= 638,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;
static idx_t a_computed_up_to = 0;
static bool a_initialized = false;

static void a_ensure_cache(idx_t n) {
    /* initial allocation and base values */
    if (!a_initialized) {
        size_t need = n + 1;
        if (need < 11) need = 11;          /* we need values up to 10 */
#if defined(__cplusplus)
        a_cache = new seq_t[need];
        for (size_t i = 0; i < need; ++i) a_cache[i] = 0;
        a_cache[0] = 1;
        for (idx_t i = 1; i < 9 && i < (idx_t)need; ++i) a_cache[i] = 0;
        if (need > 9) a_cache[9] = 1;
        if (need > 10) a_cache[10] = 1;
#else
        a_cache = (seq_t*)malloc(need * sizeof(seq_t));
        if (!a_cache) return;                 /* out of memory: abort silently */
        for (size_t i = 0; i < need; ++i) a_cache[i] = 0;
        a_cache[0] = 1;
        for (idx_t i = 1; i < 9 && i < (idx_t)need; ++i) a_cache[i] = 0;
        if (need > 9) a_cache[9] = 1;
        if (need > 10) a_cache[10] = 1;
#endif
        a_cache_size = need;
        a_computed_up_to = (need > 10) ? 10 : ((need > 9) ? 9 : 0);
        a_initialized = true;
    }

    /* resize if necessary */
    if ((size_t)(n + 1) > a_cache_size) {
        size_t new_size = n + 1;
        seq_t *new_cache = NULL;
#if defined(__cplusplus)
        new_cache = new seq_t[new_size];
        for (size_t i = 0; i < a_cache_size; ++i) new_cache[i] = a_cache[i];
        delete[] a_cache;
#else
        new_cache = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
#endif
        if (!new_cache) return; /* allocation failed: abort silently */
        /* zero remaining entries */
        for (size_t i = a_cache_size; i < new_size; ++i) new_cache[i] = 0;
        a_cache = new_cache;
        a_cache_size = new_size;
    }

    /* compute missing terms */
    if (a_computed_up_to < n) {
        idx_t start = a_computed_up_to + 1;
        for (idx_t i = start; i <= n; ++i) {
            seq_t val = 0;
            if (i >= 9)  val += a_cache[i - 9];
            if (i >= 10) val += a_cache[i - 10];
            a_cache[i] = val;
        }
        a_computed_up_to = n;
    }
}

seq_t a(idx_t n) {
    a_ensure_cache(n);
    return a_cache[n];
}
