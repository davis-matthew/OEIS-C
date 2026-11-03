/*
 * A030479  Cubes with property that all even digits occur together and all
 *          odd digits occur together.
 * https://oeis.org/A030479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A030478(n))^3, where A030478(n) is the n‑th positive
 *            integer whose cube has all even digits together and all odd
 *            digits together (decimal notation)
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: test whether the decimal digits of x have at most one
   transition from odd to even or even to odd when read from
   least‑significant to most‑significant.  This is equivalent to
   “all even digits occur together and all odd digits occur together”. */
static bool parity_blocks(seq_t x)
{
    if (x == 0) return true;          /* 0 has only even digits */

    seq_t temp = x;
    int curr = (int)(temp % 10) & 1;   /* parity of least–significant digit */
    temp /= 10;

    bool transitioned = false;
    while (temp > 0) {
        int p = (int)(temp % 10) & 1;
        if (!transitioned) {
            if (p != curr) {
                transitioned = true;
                curr = p;
            }
        } else {
            if (p != curr) return false;   /* second transition → invalid */
        }
        temp /= 10;
    }
    return true;
}

/* Dynamic cache for the cubes of the accepted numbers. */
static seq_t *cache  = NULL;   /* array of cubes */
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_len = 0;   /* number of cached terms */
static idx_t next_candidate = 0;   /* next n to test for inclusion */

/* Ensure that the cache can hold at least “need” entries. */
static void ensure_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_buf = mem_resize(seq_t, cache, new_cap);
    cache = new_buf;
    cache_cap = new_cap;
}

/* Generate more terms of the sequence until at least n terms are cached. */
static void fill_cache(idx_t n)
{
    while (cache_len < n) {
        seq_t m = (seq_t)next_candidate;
        seq_t cube = m * m * m;
        if (parity_blocks(cube)) {
            ensure_cap(cache_len + 1);
            cache[cache_len++] = cube;
        }
        ++next_candidate;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* index n is 1‑based */
    fill_cache(n);
    return cache[n - 1];
}
