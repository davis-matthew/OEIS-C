/*
 * A032925  Numbers whose set of base-4 digits is a subset of {1,2}.
 * https://oeis.org/A032925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(2n) = 4a(n‑1) + 2, a(2n+1) = 4a(n) + 1 * Initial: a(1) = 1,
 *            a(0) = 0 (auxiliary for recursion)
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 898 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static seq_t *cache = NULL;
static size_t cap = 0;

/* Helper: compute a(n) recursively with memoisation
 * n is 0‑based for internal recursion (a(0) = 0).
 */
static seq_t compute_a(idx_t n)
{
    if (n == 0)
        return 0;

    ensure_cache(n, &cache, &cap);

    if (cache[n] != 0)
        return cache[n];

    seq_t res;
    if ((n & 1) == 0) {         /* n even -> n = 2k */
        idx_t k = n >> 1;
        seq_t val = compute_a(k - 1);  /* a(k‑1), with k‑1 possibly 0 */
        res = (seq_t)4 * val + 2;
    } else {                    /* n odd  -> n = 2k+1 */
        idx_t k = n >> 1;
        seq_t val = compute_a(k);
        res = (seq_t)4 * val + 1;
    }

    cache[n] = res;
    return res;
}

/* Public entry point: a(idx_t n) with n ≥ 1 */
seq_t a(idx_t n)
{
    return compute_a(n);
}
