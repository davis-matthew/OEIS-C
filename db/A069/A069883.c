/*
 * A069883  Smallest nontrivial palindromic n-th perfect power (a(n) >1 for
 *          n>1).
 * https://oeis.org/A069883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   ipow, is_palindrome_base, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the smallest palindrome that is a perfect n‑th power.
   (a(1) = 1, a(2) = 4, a(3) = 8, a(4) = 14641, ...) */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow.h"
#include "../utils/is_palindrome_base.h"
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

seq_t a(idx_t n)
{
    /* Minimal special cases. */
    if (n == 0) return 0;   /* undefined; just for safety */
    if (n == 1) return 1;   /* trivial 1^1 */

    /* Cache of previously computed values. */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Ensure the cache has room for indices up to n. */
    ensure_cache(n, &cache, &cap);

    /* Return cached result if already computed. */
    seq_t cached = cache[n - 1];
    if (cached != (seq_t)0) return cached;

    /* Search for the minimal base whose n‑th power is a palindrome. */
    seq_t best = 0;
    for (idx_t base = 2; ; ++base) {
        seq_t sb = (seq_t)base;
        seq_t pow = ipow(sb, (seq_t)n);
        if (is_palindrome_base(pow, 10)) {
            best = pow;
            break;
        }
    }

    cache[n - 1] = best;
    return best;
}
