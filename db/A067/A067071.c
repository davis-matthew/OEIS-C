/*
 * A067071  Numbers with no zeros in their cubes such that the products of the
 *          digits of their cubes are also cubes.
 * https://oeis.org/A067071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 17, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   digit_product, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by brute‑force search: test each integer n,
   keep those whose cube contains no zero digit and whose
   digit product is a perfect cube. */
#include <stddef.h>
#ifndef __cplusplus
/* The C build treats seq_t as an unsigned integer type. */
#include <stdbool.h>
#endif

/* Utility functions supplied by the build system. */
#include "../utils/digit_product.h"
#include "../utils/floor_cube_root.h"

#ifdef __cplusplus
#include <vector>
/* In C++ builds seq_t may be an arbitrary‑precision type. */
static std::vector<seq_t> cache_vec;

/* Append a term to the cache. */
static inline void add_term(seq_t v) { cache_vec.push_back(v); }

/* Retrieve a zero‑based element from the cache. */
static inline seq_t get_term(size_t idx) { return cache_vec[idx]; }

#else
/* In C builds seq_t is a fundamental unsigned integer type. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Append a term to the cache, expanding with realloc if needed. */
static void add_term(seq_t v) {
    if (cache_size >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = v;
}

/* Retrieve a zero‑based element from the cache. */
static inline seq_t get_term(size_t idx) { return cache[idx]; }
#endif

/* The next integer to be tested.  Starts at 1. */
static idx_t next_checked = 1;

/* Return the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, return 0 */

    /* Generate terms until we have at least n of them. */
    for (;;) {
#ifdef __cplusplus
        if (cache_vec.size() >= (size_t)n) break;
#else
        if (cache_size >= (size_t)n) break;
#endif
        seq_t m = next_checked++;             /* candidate integer */

        seq_t cube = m * m * m;               /* m³ */
        seq_t prod = digit_product(cube);     /* product of cube’s digits */

        if (prod == 0) continue;              /* contains a zero digit */

        seq_t r = floor_cube_root(prod);      /* integer cube root of prod */
        if (r * r * r == prod)                /* prod is a perfect cube */
            add_term(m);                      /* store m as a sequence term */
    }

    /* Return the (n‑1)-based index. */
    return get_term((size_t)n - 1);
}
