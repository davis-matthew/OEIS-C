/*
 * A178010  Number of permutations of 1..n with no element
 *          e[i>=2]<e[1+floor((i-2)/5)] (5-way heap).
 * https://oeis.org/A178010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of 1..n that are 5‑heap ordered.
 *            This is computed recursively using the subtree sizes of a
 *            complete 5‑ary heap and the multinomial coefficient: a(n) =
 *            (n-1)! / (∏ size_i!) × ∏ a(size_i), where size_i are the sizes
 *            of the five child subtrees of the root
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binom.h"

/* Helper to compute the size of the subtree rooted at `idx`
   in a 5‑ary heap containing `n` nodes. */
static idx_t subtree_size(idx_t idx, idx_t n) {
    if (idx > n) return 0;
    idx_t sz = 1;
    for (idx_t c = 0; c < 5; ++c) {
        idx_t child = 5 * (idx - 1) + 2 + c; /* 1‑based indices */
        sz += subtree_size(child, n);
    }
    return sz;
}

/* Compute sizes of the five child subtrees of the root in a heap of `n` nodes. */
static void child_sizes(idx_t n, idx_t *sizes) {
    for (idx_t i = 0; i < 5; ++i) {
        idx_t child = i + 2;  /* indices 2..6 */
        sizes[i] = subtree_size(child, n);
    }
}

/* Cache for memoisation of a(n).  Zero means "not yet computed". */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold values up to `n`. */
static void ensure_cache(idx_t n) {
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) return; /* allocation failure, silence it */
    for (size_t i = 0; i < cache_cap; ++i) tmp[i] = cache[i];
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    free(cache);
    cache = tmp;
    cache_cap = new_cap;
}

/* Main function computing a(n). */
seq_t a(idx_t n) {
    if (n == 0 || n == 1) return (seq_t)1;

    ensure_cache(n);

    if (cache[n] != (seq_t)0) return cache[n];

    idx_t sizes[5];
    child_sizes(n, sizes);

    seq_t coeff = 1;
    seq_t prod  = 1;
    idx_t remaining = n - 1;

    for (idx_t i = 0; i < 5; ++i) {
        idx_t sz = sizes[i];
        if (sz == 0) continue;
        coeff *= binom(remaining, sz);
        remaining -= sz;
        prod *= a(sz);
    }

    seq_t result = coeff * prod;
    cache[n] = result;
    return result;
}
