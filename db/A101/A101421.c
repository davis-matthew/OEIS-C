/*
 * A101421  Numbers which are the sum of two positive cubes and divisible by
 *          7.
 * https://oeis.org/A101421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer that is divisible by 7 and can be
 *            written as the sum of two positive cubes
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   cube
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cube.h"

typedef struct {
    seq_t sum;
    idx_t a;   /* first cube index (>=1) */
    idx_t b;   /* second cube index (>=a) */
} heap_node;

static idx_t    maxA = 3000;          /* range of cube indices */
static seq_t *  cubes = NULL;         /* cubes[1..maxA] = a^3        */
static heap_node *heap = NULL;        /* binary min-heap of size maxA */
static idx_t    heap_sz = 0;

static seq_t *  seq   = NULL;         /* resulting sequence          */
static idx_t    seq_sz = 0;
static idx_t    seq_cap = 0;

static bool    initialized = false;

/* swap two heap nodes */
static inline void swap_node(heap_node *x, heap_node *y) {
    heap_node tmp = *x; *x = *y; *y = tmp;
}

/* sift up from position i */
static void heap_sift_up(idx_t i) {
    while (i > 0) {
        idx_t parent = (i - 1) >> 1;
        if (heap[i].sum >= heap[parent].sum) break;
        swap_node(&heap[i], &heap[parent]);
        i = parent;
    }
}

/* sift down from position i */
static void heap_sift_down(idx_t i) {
    while (true) {
        idx_t left  = (i << 1) + 1;
        idx_t right = left + 1;
        idx_t smallest = i;
        if (left  < heap_sz && heap[left].sum < heap[smallest].sum) smallest = left;
        if (right < heap_sz && heap[right].sum < heap[smallest].sum) smallest = right;
        if (smallest == i) break;
        swap_node(&heap[i], &heap[smallest]);
        i = smallest;
    }
}

/* push a node onto the heap */
static void heap_push(const heap_node *node) {
    heap[heap_sz++] = *node;
    heap_sift_up(heap_sz - 1);
}

/* pop and return the smallest node */
static heap_node heap_pop(void) {
    heap_node min = heap[0];
    heap[0] = heap[--heap_sz];
    if (heap_sz > 0) heap_sift_down(0);
    return min;
}

/* initialise cubes, heap and sequence containers */
static void init_sequence(void) {
    /* cubes */
    cubes = (seq_t *)malloc((maxA + 1) * sizeof(seq_t));
    for (idx_t i = 1; i <= maxA; ++i) cubes[i] = cube((seq_t)i);

    /* heap */
    heap = (heap_node *)malloc(maxA * sizeof(heap_node));
    heap_sz = 0;
    for (idx_t a = 1; a <= maxA; ++a) {
        heap_node n;
        n.a = a; n.b = a;
        n.sum = cubes[a] + cubes[a];
        heap_push(&n);
    }

    /* empty sequence */
    seq = NULL;
    seq_sz = 0;
    seq_cap = 0;
    initialized = true;
}

/* generate sequence until it contains at least target terms */
static void ensure_terms(idx_t target) {
    if (!initialized) init_sequence();

    while (seq_sz < target) {
        heap_node node = heap_pop();

        /* Unique check */
        if (seq_sz == 0 || node.sum != seq[seq_sz - 1]) {
            if ((node.sum % 7) == 0) {
                /* grow seq buffer if necessary */
                if (seq_sz == seq_cap) {
                    seq_cap = seq_cap ? seq_cap << 1 : 256;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_sz++] = node.sum;
            }
        }

        /* push next element from the same a‑list if possible */
        if (node.b < maxA) {
            ++node.b;
            node.sum = cubes[node.a] + cubes[node.b];
            heap_push(&node);
        }
    }
}

/* required API */
seq_t a(idx_t n) {
    if (n < 1) return 0;               /* undefined, but safe */
    ensure_terms(n);
    return seq[n - 1];
}
