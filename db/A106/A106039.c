/*
 * A106039  Belgian-0 numbers.
 * https://oeis.org/A106039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_belgian0_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Belgian-0 numbers: a(n) is the n-th integer m ≥ 0 such that
   there exists a remainder r with 0 ≤ r < d where d is the number
   of decimal digits of m, for which
        m − prefix(r) is divisible by the sum of the digits of m.
   Here prefix(r) is the sum of the first r digits of m (most‑significant first). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_belgian0_ull.h"

static seq_t *term_cache = NULL;
static size_t cache_cap   = 0;
static size_t term_count  = 0;
static unsigned long long next_candidate = 0;

static void grow_cache(size_t min_needed)
{
    if (cache_cap >= min_needed) return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < min_needed) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    term_cache = new_arr;
    cache_cap = new_cap;
}

static void build_to(idx_t n)
{
    while (term_count < n) {
        /* Find next Belgian‑0 number starting from next_candidate. */
        while (!is_belgian0_ull(next_candidate)) ++next_candidate;
        /* Store it. */
        grow_cache(term_count + 1);
        term_cache[term_count] = (seq_t)next_candidate;
        ++term_count;
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard */
    build_to(n);
    return term_cache[n - 1];
}
