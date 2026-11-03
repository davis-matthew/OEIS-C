/*
 * A131893  a(n) is the number of shapes of balanced trees with constant
 *          branching factor 7 and n nodes. The node is balanced if the size,
 *          measured in nodes, of each pair of its children differ by at most
 *          one node.
 * https://oeis.org/A131893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1; a(7*n+1+m) = (7 choose m) * a(n+1)^m *
 *            a(n)^(7-m)
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 22, u64 n <= 68, u128 n <= 89,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 395 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

seq_t a(idx_t n)
{
    /* static cache holding previously computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t max_comp = 0;            /* highest index computed so far */
    static const seq_t comb[8] = {
        (seq_t)1, (seq_t)7, (seq_t)21, (seq_t)35,
        (seq_t)35, (seq_t)21, (seq_t)7, (seq_t)1
    };

    /* Make sure the cache is large enough for the requested index */
    ensure_cache(n, &cache, &cap);

    /* Initialise the base cases if not yet done */
    if (max_comp == 0) {
        cache[0] = seq_one();
        if (n > 0) {
            cache[1] = seq_one();
            max_comp = 1;
        }
    }

    /* Compute any missing values up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        idx_t t = (i - 1) / 7;
        idx_t m = (i - 1) - 7 * t;          /* 0 <= m <= 7 */

        seq_t part1 = cache[t + 1];          /* a(n+1) */
        seq_t part2 = cache[t];              /* a(n)   */

        seq_t pow1   = ipow_idx(part1, (idx_t)m);
        seq_t pow2   = ipow_idx(part2, (idx_t)(7 - m));

        seq_t term = seq_mul(seq_mul(comb[m], pow1), pow2);
        cache[i] = term;
    }

    max_comp = n;
    return cache[n];
}
