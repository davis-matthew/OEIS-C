/*
 * A213726  a(n)=0 if n is in the infinite trunk of the "beanstalk" (i.e., in
 *          A179016), otherwise number of terminal nodes (leaves) in that
 *          finite branch of the beanstalk.
 * https://oeis.org/A213726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of terminal nodes (leaves) in the beanstalk tree
 *            associated with n; a(n) = 0 if n lies on the infinite trunk,
 *            a(n) = 1 if n itself is a leaf, otherwise a(n) = a(child1) +
 *            a(child2) where the two children of n are the numbers m such
 *            that m - popcount(m) = n. The implementation uses m...
 * Range:     u8 n <= 9847, u16 n <= 9847, u32 n <= 9847, u64 n <= 9847,
 *            u128 n <= 9847, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* maximum recursion depth considered finite; anything deeper is treated as
   belonging to the infinite trunk */
#define MAX_DEPTH 1000

/* maximum possible difference between a node and its child (maximum
   possible number of set bits in a 64‑bit integer). */
#define MAX_T 64

/* helper: number of 1-bits in a 64‑bit integer */
static inline int popcount_idx(idx_t x) {
    return __builtin_popcountll((unsigned long long)x);
}

/* memoisation tables */
static seq_t *cache = NULL;        /* computed values or 0 for infinite trunk */
static unsigned char *state = NULL;/* 0 = unknown, 1 = computing, 2 = computed */
static size_t cache_cap = 0;

/* enlarge cache to hold at least size elements */
static void extend_cache(size_t size) {
    if (size <= cache_cap) return;
    /* round up to nearest power of two for amortised efficiency */
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < size) new_cap <<= 1;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    state = (unsigned char*)realloc(state, new_cap * sizeof(unsigned char));
    for (size_t i = cache_cap; i < new_cap; ++i) {
        state[i] = 0;        /* unknown */
    }
    cache_cap = new_cap;
}

/* recursive helper; depth limits detection of infinite trunk */
static seq_t compute_value(idx_t n, int depth) {
    if (n >= (idx_t)cache_cap) extend_cache((size_t)(n + 1));

    if (state[n] == 2) return cache[n];
    if (depth > MAX_DEPTH) {
        cache[n] = 0;        /* infinite trunk detected by depth limit */
        state[n] = 2;
        return 0;
    }

    state[n] = 1;            /* mark as computing */

    seq_t sum = 0;
    bool infinite_child = false;
    int child_count = 0;

    for (int t = 1; t <= MAX_T; ++t) {
        idx_t child = n + (idx_t)t;
        if (popcount_idx(child) != t) continue;
        /* child is a real child of n */
        seq_t val = compute_value(child, depth + 1);
        if (val == 0) {                  /* child lies on infinite trunk */
            infinite_child = true;
            break;
        }
        sum += val;
        child_count++;
    }

    if (infinite_child) {
        cache[n] = 0;
    } else if (child_count == 0) {
        cache[n] = 1;                    /* leaf */
    } else {
        cache[n] = sum;
    }

    state[n] = 2;
    return cache[n];
}

/* public function required by the specification */
seq_t a(idx_t n) {
    return compute_value(n, 0);
}
