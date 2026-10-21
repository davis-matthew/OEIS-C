/*
 * A071995  a(1) = 1, a(2) = 0, a(n) = a(floor(n/3)) + a(n - floor(n/3)).
 * https://oeis.org/A071995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(floor(n/3)) + a(n - floor(n/3)), a(1) = 1, a(2) = 0
 * Range:     u8 n <= 635, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for memoisation ------------------------------------------------- */
static seq_t *cache    = NULL;      /* computed values */
static bool  *used     = NULL;      /* flags whether entry is initialized */
static size_t cache_cap = 0;        /* current capacity of the arrays */

/* Ensure the cache can store values up to index n (inclusive) ------------------ */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;                       /* already big enough */

    size_t new_cap = (size_t)n + 1;    /* indices 0..n */

    /* Reallocate for arbitrary‑precision seq_t and bool */
    seq_t  *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    bool   *new_used  = (bool*)realloc(used,  new_cap * sizeof(bool));

    if (!new_cache || !new_used) {
        abort();                       /* allocation failed */
    }

    /* Update pointers and capacity */
    cache    = new_cache;
    used     = new_used;
    /* Initialise new entries as uncomputed */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        cache[i] = 0;
        used[i]  = false;
    }

    /* Set known initial values (they remain unchanged after a resize) */
    if (new_cap > 1) {
        cache[1] = 1;
        used[1]  = true;
    }
    if (new_cap > 2) {
        cache[2] = 0;
        used[2]  = true;
    }

    cache_cap = new_cap;
}

/* Recursive helper that uses memoisation ------------------------------------- */
static seq_t compute_val(idx_t n)
{
    if (n < cache_cap && used[n])
        return cache[n];

    ensure_cache(n);

    seq_t value;
    if (n == 1) {
        value = 1;
    } else if (n == 2) {
        value = 0;
    } else {
        idx_t k = n / 3;                     /* floor(n/3) */
        value = compute_val(k) + compute_val(n - k);
    }

    cache[n] = value;
    used[n]  = true;
    return value;
}

/* Public interface ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined by OEIS, return 0 as a safe fallback */
        return 0;

    return compute_val(n);
}
