/*
 * A026166  For n >= 2, let h=floor((n-1)/2), L=n-h, R=n+h; then a(L)=n if
 *          a(L) not yet defined, otherwise a(R)=n; thus |a(n)-n| =
 *          floor((1/2)*(a(n)-1)).
 * https://oeis.org/A026166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generates permutation defined by OEIS A026166 via
   assignment rule:
   For n >= 2, let h = floor((n-1)/2), L = n - h, R = n + h;
   if a[L] is not set, set a[L] = n, else set a[R] = n.
   The sequence is a permutation of positive integers, a(1)=1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Types supplied by the build system */
typedef seq_t seq_t;
typedef idx_t idx_t;

/* Static cache for computed values.
   a_cache[i] holds a(i) for i >= 1 (index 0 unused).
   filled[i] == 1 when a_cache[i] has been set. */
static seq_t *a_cache     = NULL;
static uint8_t *filled    = NULL;   /* 0 = unset, 1 = set */
static idx_t cache_cap    = 0;      /* next free index in the arrays (1‑based) */
static idx_t current_n    = 2;      /* next n to process (a[1] already set) */

/* Ensure the cache can address index 'idx' (1‑based).  Uses C cast for
   C++ compatibility. */
static void ensure_capacity(idx_t idx)
{
    if (cache_cap > idx) return;          /* already big enough */

    /* Grow until we have capacity for idx+1 slots (1‑based). */
    size_t newcap = cache_cap ? (size_t)cache_cap : 16;
    while (newcap <= (size_t)idx) newcap <<= 1;

    seq_t *new_a = (seq_t *)realloc(a_cache, newcap * sizeof(seq_t));
    uint8_t *new_filled = (uint8_t *)realloc(filled, newcap * sizeof(uint8_t));
    if (!new_a || !new_filled) {
        /* Allocation failed; abort the program. */
        abort();
    }

    /* Mark newly allocated slots as unset. */
    for (size_t i = cache_cap; i < newcap; ++i) {
        new_filled[i] = 0;
    }

    a_cache = new_a;
    filled  = new_filled;
    cache_cap = (idx_t)newcap;
}

/* Initialise the cache with a(1) = 1. */
static void init_cache(void)
{
    if (a_cache) return;        /* already initialised */

    size_t initial = 16;
    a_cache = (seq_t *)calloc(initial, sizeof(seq_t));
    filled  = (uint8_t *)calloc(initial, sizeof(uint8_t));
    if (!a_cache || !filled) abort();

    cache_cap = (idx_t)initial;
    a_cache[1] = (seq_t)1;      /* a(1) = 1 */
    filled[1]  = 1;
    current_n  = 2;             /* next n to process */
}

/* Public function: returns a(n).  Call in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range; not used */

    init_cache();
    ensure_capacity(n);

    /* Compute terms up to n if they are not yet known. */
    while (!filled[n]) {
        idx_t h   = (current_n - 1) / 2;
        idx_t L   = current_n - h;
        idx_t R   = current_n + h;

        ensure_capacity(L);
        ensure_capacity(R);

        if (!filled[L]) {
            a_cache[L]  = (seq_t)current_n;
            filled[L]   = 1;
        } else {
            a_cache[R]  = (seq_t)current_n;
            filled[R]   = 1;
        }
        ++current_n;
    }

    return a_cache[n];
}
