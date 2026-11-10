/*
 * A218548  Positions of records in A213726.
 * https://oeis.org/A218548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = position (from offset 0) of the n-th record (strict new
 *            maximum) in A213726; computed by scanning with a memoized
 *            beanstalk leaf-count recursion
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

#define A218548_MAX_DEPTH 1000
#define A218548_MAX_T 64

static seq_t *A218548_cache = NULL;
static unsigned char *A218548_state = NULL;
static size_t A218548_cap = 0;

static void A218548_extend(size_t size) {
    if (size <= A218548_cap) return;
    size_t new_cap = A218548_cap ? A218548_cap : 1;
    while (new_cap < size) new_cap <<= 1;
    A218548_cache = mem_resize(seq_t, A218548_cache, new_cap);
    A218548_state = (unsigned char*)realloc(A218548_state, new_cap * sizeof(unsigned char));
    for (size_t i = A218548_cap; i < new_cap; ++i) {
        A218548_state[i] = 0;
        A218548_cache[i] = 0;
    }
    A218548_cap = new_cap;
}

static seq_t A218548_val(idx_t n, int depth) {
    if (n >= (idx_t)A218548_cap) A218548_extend((size_t)(n + 1));
    if (A218548_state[n] == 2) return A218548_cache[n];
    if (depth > A218548_MAX_DEPTH) {
        A218548_cache[n] = 0;
        A218548_state[n] = 2;
        return 0;
    }
    A218548_state[n] = 1;
    seq_t sum = 0;
    bool inf = false;
    int cc = 0;
    for (int t = 1; t <= A218548_MAX_T; ++t) {
        idx_t child = n + (idx_t)t;
        if (__builtin_popcountll((unsigned long long)child) != t) continue;
        seq_t v = A218548_val(child, depth + 1);
        if (v == 0) { inf = true; break; }
        sum += v;
        cc++;
    }
    seq_t r;
    if (inf) r = 0;
    else if (cc == 0) r = 1;
    else r = sum;
    /* cache may have been reallocated during recursion */
    A218548_cache[n] = r;
    A218548_state[n] = 2;
    return r;
}

static idx_t *A218548_recs = NULL;
static size_t A218548_nrecs = 0, A218548_reccap = 0;
static idx_t A218548_scan = 0;
static bool A218548_have = false;
static seq_t A218548_best = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (A218548_nrecs < (size_t)n) {
        seq_t v = A218548_val(A218548_scan, 0);
        if (!A218548_have || v > A218548_best) {
            A218548_have = true;
            A218548_best = v;
            if (A218548_nrecs >= A218548_reccap) {
                A218548_reccap = A218548_reccap ? A218548_reccap * 2 : 64;
                A218548_recs = (idx_t*)realloc(A218548_recs, A218548_reccap * sizeof(idx_t));
            }
            A218548_recs[A218548_nrecs++] = A218548_scan;
        }
        A218548_scan++;
    }
    return (seq_t)A218548_recs[n - 1];
}
