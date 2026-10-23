/*
 * A044508  Numbers n such that string 4,1 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer n such that the base‑6 representation of
 *            n contains the substring “41” and the base‑6 representation of
 *            n+1 does not. The implementation searches sequentially for terms
 *            and memoises them
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Test whether a number in base 6 contains the consecutive digits 4,1. */
static bool has41_in_base6(seq_t n) {
    /* at least two digits required */
    if (n < 6) return false;

    seq_t prev = n % 6;
    n /= 6;
    while (n > 0) {
        seq_t curr = n % 6;
        if (curr == 4 && prev == 1) return true; /* pattern 4 (more sig) 1 (less sig) */
        prev = curr;
        n /= 6;
    }
    return false;
}

#ifdef __cplusplus
/* C++ implementation – use std::vector for arbitrary‑precision types. */
static std::vector<seq_t> cache;
static idx_t next = 1;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for n < 1, harmless */

    while (cache.size() < n) {
        if (has41_in_base6((seq_t)next) && !has41_in_base6((seq_t)(next + 1))) {
            cache.push_back((seq_t)next);
        }
        ++next;
    }
    return cache[n - 1];
}
#else
/* C implementation – use realloc. */
static seq_t *cache = NULL;
static size_t cap   = 0;
static size_t len   = 0;
static idx_t  next  = 1;

static void ensure_capacity(void) {
    if (len == cap) {
        size_t new_cap = (cap > 0) ? cap * 2 : 1024;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory – terminate */
        cache = tmp;
        cap   = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for n < 1, harmless */

    while (len < n) {
        seq_t cand = (seq_t)next;
        if (has41_in_base6(cand) && !has41_in_base6(cand + 1)) {
            ensure_capacity();
            cache[len++] = cand;
        }
        ++next;
    }
    return cache[n - 1];
}
#endif
