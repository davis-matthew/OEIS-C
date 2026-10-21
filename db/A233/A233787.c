/*
 * A233787  Number of (n+1)X(3+1) 0..3 arrays with every 2X2 subblock having
 *          the sum of the squares of all six edge and diagonal differences
 *          equal to 11.
 * https://oeis.org/A233787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays over {0,1,2,3} in which every
 *            2×2 subblock has the * sum of the squares of its six edge and
 *            diagonal differences equal to 11. * Computation: transfer‑matrix
 *            method with a 256‑state graph (each row is a base‑4 4‑digit
 *            number)
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/allowed.h"

/* adjacency list for the 256‑state graph */
struct transition {
    uint16_t *next;   /* array of succeeding states */
    size_t   len;     /* current number of edges */
    size_t   cap;     /* allocated capacity */
};
static struct transition trans[256];

/* DP vectors for the current and previous row counts */
static seq_t *dp_prev = NULL;
static seq_t *dp_next = NULL;

/* memoised a(n) values */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t max_computed = 0;

/* add an edge to the adjacency list */
static void push_trans(uint16_t row, uint16_t next) {
    struct transition *t = &trans[row];
    if (t->len == t->cap) {
        size_t newcap = t->cap ? t->cap * 2 : 8;
        uint16_t *tmp = (uint16_t*)realloc(t->next, newcap * sizeof(uint16_t));
        t->next = tmp;
        t->cap = newcap;
    }
    t->next[t->len++] = next;
}

/* build the adjacency list of the automaton */
static void build_transitions(void) {
    for (int r = 0; r < 256; ++r) {
        trans[r].next = NULL;
        trans[r].len  = 0;
        trans[r].cap  = 0;
    }
    for (uint16_t p = 0; p < 256; ++p)
        for (uint16_t s = 0; s < 256; ++s)
            if (allowed(p, s))
                push_trans(p, s);
}

/* initialize once */
static void init(void) {
    if (dp_prev) return;          /* already done */

    build_transitions();

    dp_prev = (seq_t*)malloc(256 * sizeof(seq_t));
    dp_next = (seq_t*)malloc(256 * sizeof(seq_t));
    for (int i = 0; i < 256; ++i) {
        dp_prev[i] = 1;           /* single row */
        dp_next[i] = 0;
    }

    cache     = (seq_t*)malloc(8 * sizeof(seq_t));
    cache_cap = 8;
    cache[0]  = 0;                /* unused */
    max_computed = 0;
}

/* the requested sequence value */
seq_t a(idx_t n) {
    if (n == 0) return 0;         /* offset is 1; a(0) undefined */

    init();

    if (n < cache_cap && n <= max_computed) return cache[n];

    /* compute consecutively up to n */
    for (idx_t step = max_computed + 1; step <= n; ++step) {
        /* zero the next DP vector */
        for (int i = 0; i < 256; ++i) dp_next[i] = 0;

        /* transition from dp_prev to dp_next */
        for (int p = 0; p < 256; ++p) {
            seq_t val = dp_prev[p];
            if (val == 0) continue;
            struct transition *t = &trans[p];
            for (size_t k = 0; k < t->len; ++k)
                dp_next[t->next[k]] += val;
        }

        /* sum for a(step) */
        seq_t sum = 0;
        for (int i = 0; i < 256; ++i) sum += dp_next[i];

        /* enlarge cache if necessary */
        if (step >= cache_cap) {
            size_t cap = cache_cap ? cache_cap * 2 : 8;
            while (cap <= step) cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = cap;
        }
        cache[step] = sum;

        /* prepare for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
    }

    max_computed = n;
    return cache[n];
}
