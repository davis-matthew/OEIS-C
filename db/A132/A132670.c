/*
 * A132670  a(1)=1, a(n) = 6*a(n-1) if the minimal positive integer not yet in
 *          the sequence is greater than a(n-1), else a(n) = a(n-1) - 1.
 * https://oeis.org/A132670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(n) = 6*a(n-1) if the minimal positive integer not yet
 *            in the sequence is greater than a(n-1), else a(n) = a(n-1) - 1
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a cache of computed terms and a list of
   already used values.  It works in any order of calls.  For C++ builds
   seq_t can be an arbitrary-precision class; a std::vector is used
   to store the values to avoid the undefined behaviour of realloc on
   non‑POD types.  In a C build plain malloc/realloc is used. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Global state ------------------------------------------------------- */
static seq_t current;          /* last generated value                  */
static seq_t min_unused;       /* smallest positive integer not used yet */

#ifdef __cplusplus
static std::vector<seq_t> cache;     /* sequence values 1‑based             */
static std::vector<seq_t> used_vals; /* list of values already seen         */
#else
static seq_t *cache     = NULL;  /* sequence values 1‑based (array 0‑based) */
static size_t cache_cap  = 0;
static size_t cache_size = 0;

static seq_t *used_vals  = NULL;  /* list of values already seen         */
static size_t used_cap   = 0;
static size_t used_cnt   = 0;
#endif

/* --------------------------------------------------------------------- */
/* Helper to test whether a value has already appeared in the sequence.  */
static bool used_contains(const seq_t val) {
#ifdef __cplusplus
    for (const auto &v : used_vals) {
        if (v == val) return true;
    }
    return false;
#else
    for (size_t i = 0; i < used_cnt; ++i) {
        if (used_vals[i] == val) return true;
    }
    return false;
#endif
}

/* --------------------------------------------------------------------- */
/* Expand the C‑style cache if necessary (C mode only). */
#ifndef __cplusplus
static void grow_cache(size_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
    if (cache == NULL) abort();
}

static void grow_used(size_t need) {
    if (need <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    used_vals = (seq_t *)realloc(used_vals, new_cap * sizeof(seq_t));
    used_cap = new_cap;
    if (used_vals == NULL) abort();
}
#endif

/* --------------------------------------------------------------------- */
/* Generate the sequence up to index n (1‑based).  The function works
   correctly regardless of whether n is larger or smaller than values
   already cached. */
static void generate_up_to(idx_t n) {
    if (n < 1) return;

#ifdef __cplusplus
    if (cache.empty()) {
        /* Initialisation for the first call. */
        cache.push_back((seq_t)1);
        used_vals.push_back((seq_t)1);
        current = (seq_t)1;
        min_unused = (seq_t)2;
    }

    while (cache.size() < (size_t)n) {
        /* Compute next term according to the rule. */
        seq_t next = (min_unused > current) ? current * (seq_t)6
                                            : current - (seq_t)1;
        current = next;

        cache.push_back(current);
        used_vals.push_back(current);

        if (current == min_unused) {
            /* Advance min_unused until it points to an unused integer. */
            do {
                ++min_unused;
            } while (used_contains(min_unused));
        }
    }
#else /* C mode */
    if (cache_size == 0) {
        /* Initialisation for the first call. */
        grow_cache(64);
        cache[0] = (seq_t)1;
        cache_size = 1;

        grow_used(64);
        used_vals[0] = (seq_t)1;
        used_cnt = 1;

        current = (seq_t)1;
        min_unused = (seq_t)2;
    }

    while (cache_size < (size_t)n) {
        /* Compute next term according to the rule. */
        seq_t next = (min_unused > current) ? current * (seq_t)6
                                            : current - (seq_t)1;
        current = next;

        cache[cache_size] = current;
        cache_size++;

        used_vals[used_cnt] = current;
        used_cnt++;

        if (current == min_unused) {
            do {
                ++min_unused;
            } while (used_contains(min_unused));
        }
    }
#endif
}

/* --------------------------------------------------------------------- */
/* Public sequence function. Returns a(n) for n >= 1.                     */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* undefined, but safe */
    generate_up_to(n);
#ifdef __cplusplus
    return cache[(size_t)(n - 1)];
#else
    return cache[(size_t)(n - 1)];
#endif
}
