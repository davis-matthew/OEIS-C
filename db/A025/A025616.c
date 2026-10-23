/*
 * A025616  Numbers of form 3^i*10^j, with i, j >= 0.
 * https://oeis.org/A025616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 31, u32 n <= 112, u64 n <= 419,
 *            u128 n <= 1615, big any n
 * Helpers:   seq_t_max
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A025616 – Numbers of the form 3^i · 10^j (i, j ≥ 0), generated in
    increasing order using a min‑heap.  The function memoises the
    sequence so that calls in any order return the correct value. */

#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "../utils/seq_t_max.h"

/* Safe multiplication for bounded integral types.  For larger types
   (e.g. arbitrary‑precision) we perform the multiplication directly. */
static inline bool safe_mul(seq_t a, unsigned int m, seq_t *res) {
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        seq_t mx = seq_t_max();
        if (mx / m < a) return false;          /* overflow would occur */
    }
    *res = a * m;                              /* works for all seq_t */
    return true;
}

/* Node used in the priority queue (min‑heap). */
struct node {
    seq_t val;
    unsigned int i;   /* exponent of 3   */
    unsigned int j;   /* exponent of 10  */
};

/* Limits on exponents sufficient for n up to ~20000 terms. */
#define MAX_EXP 300
static bool visited[MAX_EXP + 1][MAX_EXP + 1];

/* Memoised sequence storage. */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Min‑heap for the frontier of candidates. */
static struct node *heap     = NULL;
static size_t heap_len  = 0;
static size_t heap_cap  = 0;
static bool initialized = false;

/* Utility functions for the heap. */
static void heap_swap(struct node *a, struct node *b) {
    struct node t = *a; *a = *b; *b = t;
}

static void heapify_up(size_t idx) {
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].val < heap[idx].val) break;
        heap_swap(&heap[parent], &heap[idx]);
        idx = parent;
    }
}

static void heapify_down(size_t idx) {
    while (1) {
        size_t left  = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;

        if (left  < heap_len && heap[left].val  < heap[smallest].val)
            smallest = left;
        if (right < heap_len && heap[right].val < heap[smallest].val)
            smallest = right;
        if (smallest == idx) break;

        heap_swap(&heap[idx], &heap[smallest]);
        idx = smallest;
    }
}

static void heap_push(seq_t val, unsigned int i, unsigned int j) {
    if (heap_len == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap << 1 : 16;
        struct node *np = (struct node *)realloc(heap,
                          new_cap * sizeof(struct node));
        if (!np) abort();
        heap = np;
        heap_cap = new_cap;
    }
    heap[heap_len].val = val;
    heap[heap_len].i   = i;
    heap[heap_len].j   = j;
    heapify_up(heap_len++);
}

static struct node heap_pop(void) {
    struct node top = heap[0];
    heap[0] = heap[--heap_len];
    heapify_down(0);
    return top;
}

/* Public function generating the sequence. */
seq_t a(idx_t n) {
    if (!initialized) {
        /* First call: initialise data structures. */
        for (size_t i = 0; i <= MAX_EXP; ++i)
            for (size_t j = 0; j <= MAX_EXP; ++j)
                visited[i][j] = false;
        heap_push((seq_t)1, 0, 0);          /* 3^0 * 10^0 = 1 */
        visited[0][0] = true;
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        initialized = true;
    }

    while (cache_len < n) {
        struct node cur = heap_pop();

        /* Store extracted value in cache. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap << 1;
            seq_t *np = (seq_t *)realloc(cache,
                          new_cap * sizeof(seq_t));
            if (!np) abort();
            cache = np;
            cache_cap = new_cap;
        }
        cache[cache_len++] = cur.val;

        /* Generate successors (i+1, j) and (i, j+1). */
        if (cur.i + 1 <= MAX_EXP && !visited[cur.i + 1][cur.j]) {
            seq_t v;
            if (safe_mul(cur.val, 3, &v)) {
                visited[cur.i + 1][cur.j] = true;
                heap_push(v, cur.i + 1, cur.j);
            }
        }
        if (cur.j + 1 <= MAX_EXP && !visited[cur.i][cur.j + 1]) {
            seq_t v;
            if (safe_mul(cur.val, 10, &v)) {
                visited[cur.i][cur.j + 1] = true;
                heap_push(v, cur.i, cur.j + 1);
            }
        }
    }

    return cache[n - 1];
}
