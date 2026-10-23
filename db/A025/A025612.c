/*
 * A025612  Numbers of form 2^i*10^j, with i, j >= 0.
 * https://oeis.org/A025612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 176, u64 n <= 659,
 *            u128 n <= 2549, big any n
 * Helpers:   cmp_seq_t, will_overflow_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A025612: Numbers of the form 2^i * 10^j (i,j >= 0) in increasing order.
   The implementation uses a min‑heap to generate values in sorted order,
   avoiding duplicates with a visited table indexed by (i,j).  The function
   is memoised so any order of calls produces the correct result. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/cmp_seq_t.h"    /* provides int cmp_seq_t(const void*,const void*) */
#include "../utils/will_overflow_mul.h"

#define VIS_MAX 2048   /* safe bound for i,j when n <= 10000 */
#define HEAP_CAP 40000 /* maximum heap size required for the bounds */

struct HeapNode {
    seq_t val;
    idx_t i;
    idx_t j;
};

/* Global state for memoisation */
static bool      visited[VIS_MAX][VIS_MAX] = {0};
static struct HeapNode heap[HEAP_CAP];
static size_t    heap_size = 0;

static seq_t *cache = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;
static int     initialized = 0;

/* Compare two seq_t values using the provided comparison function */
static inline int cmp_seq(const void *a, const void *b)
{
    return cmp_seq_t(a, b);
}

/* Swap two heap nodes */
static inline void swap_nodes(struct HeapNode *a, struct HeapNode *b)
{
    struct HeapNode tmp = *a; *a = *b; *b = tmp;
}

/* Push a node onto the min‑heap */
static inline void heap_push(struct HeapNode node)
{
    if (heap_size >= HEAP_CAP) return;   /* safety guard */
    heap[heap_size++] = node;
    size_t idx = heap_size - 1;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (cmp_seq(&heap[idx].val, &heap[parent].val) >= 0)
            break;
        swap_nodes(&heap[idx], &heap[parent]);
        idx = parent;
    }
}

/* Pop the minimum node from the heap */
static inline struct HeapNode heap_pop(void)
{
    struct HeapNode min = heap[0];
    heap[0] = heap[--heap_size];
    size_t idx = 0;
    while (1) {
        size_t left  = idx * 2 + 1;
        size_t right = left + 1;
        if (left >= heap_size) break;
        size_t smallest = left;
        if (right < heap_size &&
            cmp_seq(&heap[right].val, &heap[left].val) < 0)
            smallest = right;
        if (cmp_seq(&heap[smallest].val, &heap[idx].val) >= 0)
            break;
        swap_nodes(&heap[smallest], &heap[idx]);
        idx = smallest;
    }
    return min;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!initialized) {
        /* initialisation */
        cache_cap = 64;                                    /* initial cache capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 1;
        cache[0] = (seq_t)1;                               /* a(1) = 1 */
        visited[0][0] = true;

        /* push the first neighbours (i=1,j=0) and (i=1,j=1) */
        struct HeapNode n1 = { (seq_t)2, 1, 0 };            /* 2^1 * 10^0 = 2 */
        struct HeapNode n2 = { (seq_t)10, 1, 1 };           /* 2^1 * 10^1 = 10 */
        heap_push(n1);
        heap_push(n2);
        heap_size = 2; /* redundant but explicit */

        initialized = 1;
    }

    /* extend cache until it contains n terms */
    while (cache_len < n) {
        if (heap_size == 0) break;   /* safety guard, should not happen */

        struct HeapNode cur = heap_pop();

        /* append current value */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { free(cache); return 0; }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = cur.val;

        /* generate children */
        idx_t i = cur.i;
        idx_t j = cur.j;

        /* child 1: (i+1, j) via *2 */
        if (i + 1 < VIS_MAX && !visited[i + 1][j]) {
            visited[i + 1][j] = true;
            if (!will_overflow_mul(cur.val, 2U)) {
                struct HeapNode child = { cur.val * 2, i + 1, j };
                heap_push(child);
            }
        }

        /* child 2: (i+1, j+1) via *10 */
        if (i + 1 < VIS_MAX && j + 1 < VIS_MAX && !visited[i + 1][j + 1]) {
            visited[i + 1][j + 1] = true;
            if (!will_overflow_mul(cur.val, 10U)) {
                struct HeapNode child = { cur.val * 10, i + 1, j + 1 };
                heap_push(child);
            }
        }
    }

    return cache[n - 1];
}
