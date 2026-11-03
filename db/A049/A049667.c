/*
 * A049667  a(n) = Fibonacci(7*n)/13.
 * https://oeis.org/A049667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 637 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n)=29*a(n-1)+a(n-2) for n>1 */

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

static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;
static idx_t seq_computed = 1; /* highest index already computed */

seq_t a(idx_t n)
{
    /* Handle the base cases directly and initialise cache if needed */
    if (n == 0) return 0;
    if (n == 1) return 1;

    if (seq_cache == NULL) {
        /* Allocate space for at least indices 0 and 1 */
        ensure_cache(1, &seq_cache, &seq_cap);
        seq_cache[0] = 0;
        seq_cache[1] = 1;
    }

    if ((idx_t)seq_cap <= n) {
        /* Ensure capacity for indices up to n */
        ensure_cache(n, &seq_cache, &seq_cap);
    }

    if (n > seq_computed) {
        for (idx_t i = seq_computed + 1; i <= n; ++i) {
            seq_cache[i] = ((seq_t)29 * seq_cache[i - 1]) + seq_cache[i - 2];
        }
        seq_computed = n;
    }

    return seq_cache[n];
}
