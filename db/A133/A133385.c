/*
 * A133385  Number of permutations of n elements divided by the number of
 *          (binary) heaps on n+1 elements.
 * https://oeis.org/A133385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 30, u64 n <= 54, u128 n <= 102,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A133385 – Number of permutations of n elements divided by the number
 * of min‑heaps on n+1 elements.
 *
 * Formula:   a(n) = n! / H_{n+1}
 * Recurrence: a(0)=a(1)=1.
 * For n ≥ 2, let m = n+1, let L = heap_left_size(m) be the size of the left
 * subtree of a complete binary heap with m nodes, and let R = m-1-L.
 * Then   a(n) = a(L-1) * a(R-1) * L * R     (L,R≥1 for n≥2).
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

/* --- Helper: number of nodes in the left subtree of a complete binary heap --- */
static idx_t heap_left_size(idx_t k)
{
    if (k <= 1) return 0;                       /* only root */
    idx_t h = floor_log2_uint64(k);             /* height (0‑based) */

    /* nodes above last level */
    idx_t nodes_above_last = ((idx_t)1 << h) - 1;
    idx_t last_level_nodes = k - nodes_above_last;

    /* full nodes in left subtree (excluding last level) */
    idx_t left_full = ((idx_t)1 << (h - 1)) - 1;

    /* nodes from the last level that go to left subtree */
    idx_t cap_left_last = ((idx_t)1 << (h - 1));
    idx_t left_last = (last_level_nodes < cap_left_last)
                     ? last_level_nodes
                     : cap_left_last;

    return left_full + left_last;
}

/* --- Memoisation storage --- */
static seq_t   *a_cache   = NULL;
static uint8_t *a_computed = NULL;
static idx_t    a_size    = 0;

/* Ensure the cache is large enough for index n. */
static void ensure_cache(idx_t n)
{
    if (n < a_size) return;
    idx_t new_size = (n * 3) / 2 + 10;               /* grow a bit */

    seq_t   *tmp_cache   = (seq_t   *)realloc(a_cache, new_size * sizeof(seq_t));
    uint8_t *tmp_computed = (uint8_t *)realloc(a_computed, new_size * sizeof(uint8_t));
    if (!tmp_cache || !tmp_computed) abort();

    for (idx_t i = a_size; i < new_size; ++i) {
        tmp_cache[i]   = (seq_t)0;
        tmp_computed[i] = 0;
    }

    a_cache     = tmp_cache;
    a_computed = tmp_computed;
    a_size     = new_size;
}

/* Recursive computation with memoisation. */
static seq_t a_rec(idx_t n)
{
    if (n <= 1) return (seq_t)1;              /* a(0)=a(1)=1 */

    if (a_computed[n]) return a_cache[n];

    idx_t m = n + 1;                          /* heap size */
    idx_t L = heap_left_size(m);
    idx_t R = m - 1 - L;                       /* R ≥ 1 for n ≥ 2 */

    /* Recursively compute children */
    seq_t left  = a_rec(L - 1);
    seq_t right = a_rec(R - 1);

    seq_t res = left * right;
    res *= (seq_t)L;
    res *= (seq_t)R;

    a_cache[n]     = res;
    a_computed[n]  = 1;
    return res;
}

/* Public entry point required by the framework. */
seq_t a(idx_t n)
{
    /* initialise cache once with the two base values */
    if (a_cache == NULL) {
        idx_t init_size = 4;
        a_cache      = (seq_t   *)calloc(init_size, sizeof(seq_t));
        a_computed  = (uint8_t *)calloc(init_size, sizeof(uint8_t));
        if (!a_cache || !a_computed) abort();
        a_size = init_size;
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)1;
        a_computed[0] = a_computed[1] = 1;
    }

    ensure_cache(n);
    return a_rec(n);
}
