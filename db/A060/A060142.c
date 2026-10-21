/*
 * A060142  Ordered set S defined by these rules: 0 is in S and if x is in S
 *          then 2x+1 and 4x are in S.
 * https://oeis.org/A060142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1383, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A060142 – Ordered set S where 0∈S and if x∈S then 2x+1,4x∈S
 *
 *  Exactly follows the set generation: start with 0, then
 *  repeatedly take the smallest unprocessed element, output it,
 *  and insert its children (2x+1) and (4x, except that 4*0=0 is
 *  skipped).  A min‑heap keeps the values in sorted order.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- heap for seq_t values ---------- */
static seq_t *heap = NULL;
static size_t heap_size = 0, heap_cap = 0;

static void heap_init(void) {
    heap_cap = 16;
    heap_size = 0;
    heap = (seq_t *)malloc(sizeof(seq_t) * heap_cap);
}

static void heap_grow(void) {
    heap_cap <<= 1;
    heap = (seq_t *)realloc(heap, sizeof(seq_t) * heap_cap);
}

static void heap_push(seq_t val) {
    if (heap_size == heap_cap) heap_grow();
    size_t i = heap_size++;
    heap[i] = val;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p] <= heap[i]) break;
        seq_t tmp = heap[p]; heap[p] = heap[i]; heap[i] = tmp;
        i = p;
    }
}

static seq_t heap_pop(void) {
    seq_t ret = heap[0];
    heap_size--;
    if (heap_size > 0) {
        heap[0] = heap[heap_size];
        size_t i = 0;
        while (1) {
            size_t l = i * 2 + 1;
            size_t r = l + 1;
            size_t smallest = i;
            if (l < heap_size && heap[l] < heap[smallest]) smallest = l;
            if (r < heap_size && heap[r] < heap[smallest]) smallest = r;
            if (smallest == i) break;
            seq_t tmp = heap[i]; heap[i] = heap[smallest]; heap[smallest] = tmp;
            i = smallest;
        }
    }
    return ret;
}

/* ---------- array holding generated sequence values ---------- */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0, seq_cap = 0;

static void seq_init(void) {
    seq_cap = 16;
    seq_len = 0;
    seq_arr = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
}

static void seq_grow(void) {
    seq_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, sizeof(seq_t) * seq_cap);
}

/* Generate terms until the n‑th one is available. */
static void ensure_generated(idx_t n) {
    if (heap == NULL) {
        heap_init();
        heap_push((seq_t)0);          /* seed the heap with the initial element 0 */
    }
    if (seq_arr == NULL) seq_init();

    while (seq_len <= n) {
        seq_t x = heap_pop();
        if (seq_len == seq_cap) seq_grow();
        seq_arr[seq_len++] = x;

        if (x == (seq_t)0) {
            heap_push((seq_t)1);      /* 4*0 would be 0 again – skip it */
        } else {
            seq_t child1 = x * (seq_t)2 + (seq_t)1; /* 2x + 1 */
            seq_t child2 = x * (seq_t)4;            /* 4x */
            heap_push(child1);
            heap_push(child2);
        }
    }
}

seq_t a(idx_t n) {
    ensure_generated(n);
    return seq_arr[n];
}
