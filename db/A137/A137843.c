/*
 * A137843  Define S(1) = {1}, S(n+1) = S(n) U S(n) if a(n) is even, S(n+1) =
 *          S(n) U (n+1) U S(n) if a(n) is odd. Sequence {a(n), n >= 1} is
 *          limit as n approaches infinity of S(n). (U represents
 *          concatenation.).
 * https://oeis.org/A137843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9723, u16 n <= 9723, u32 n <= 9723, u64 n <= 9723,
 *            u128 n <= 9723, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 1.
   For n ≥ 2, let L1 = 1 and for k ≥ 2
     Lk = 2·L(k‑1) + (a(k‑1) is odd ? 1 : 0).
   Let S(k) = S(k‑1) · S(k‑1) if a(k‑1) is even,
            S(k) = S(k‑1) · k · S(k‑1) if a(k‑1) is odd.
   The value a(n) is the element at position n in S(k) where L(k‑1) < n ≤ L(k). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache_vec;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
#endif

/* Forward declaration */
static seq_t get_a(idx_t n);

/* Ensure that the cache can hold values up to index n */
static void ensure_cache(idx_t n) {
#ifdef __cplusplus
    if ((size_t)n < cache_vec.size()) return;
    cache_vec.resize((size_t)n + 1, seq_t(0));
#else
    if ((size_t)n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
    seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!ptr) exit(EXIT_FAILURE);
    /* zero new elements */
    memset(ptr + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = ptr;
    cache_cap = new_cap;
#endif
}

/* Recursive descent inside a particular construction step */
static seq_t find_a_in_step(idx_t pos, idx_t step, idx_t len) {
    if (step == 1) return (seq_t)1;

    seq_t prev_val = get_a(step - 1);
    bool odd = ((prev_val % (seq_t)2) != (seq_t)0);
    idx_t prev_len = (len - (odd ? 1 : 0)) >> 1;   /* 2*prev_len + odd = len */

    if (odd) {                                   /* S = L · step · R */
        if (pos == prev_len + 1)
            return (seq_t)step;                   /* middle element is the step number */
        if (pos <= prev_len)
            return find_a_in_step(pos, step - 1, prev_len);
        return find_a_in_step(pos - (prev_len + 1), step - 1, prev_len);
    } else {                                      /* S = L · R */
        if (pos <= prev_len)
            return find_a_in_step(pos, step - 1, prev_len);
        return find_a_in_step(pos - prev_len, step - 1, prev_len);
    }
}

/* Main function to compute a(n), with memoisation */
static seq_t get_a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* out of bounds, but safe guard */
    ensure_cache(n);
#ifdef __cplusplus
    if (cache_vec[(size_t)n] != seq_t(0)) return cache_vec[(size_t)n];
    if (n == 1) {
        cache_vec[1] = (seq_t)1;
        return (seq_t)1;
    }
#else
    if (cache[(size_t)n] != 0) return cache[(size_t)n];
    if (n == 1) {
        cache[1] = (seq_t)1;
        return (seq_t)1;
    }
#endif

    /* Find the construction step in which position n first appears */
    idx_t step = 1;
    idx_t len = 1;                       /* L1 */
    while (len < n) {
        seq_t prev_val = get_a(step);    /* a(step) is already known or computed */
        bool odd = ((prev_val % (seq_t)2) != (seq_t)0);
        len = (len << 1) + (odd ? 1 : 0);
        step++;
    }

    seq_t val = find_a_in_step(n, step, len);
#ifdef __cplusplus
    cache_vec[(size_t)n] = val;
#else
    cache[(size_t)n] = val;
#endif
    return val;
}

/* External linkage as required by the build system */
seq_t a(idx_t n) {
    return get_a(n);
}
