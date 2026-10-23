/*
 * A009388  If a, b in sequence, so is a*b-1.
 * https://oeis.org/A009388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 378, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   prod_minus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A009388: numbers closed under a*b‑1,
   generated lazily with a min‑heap of pair candidates. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/prod_minus_one.h"

/* node in the min‑heap, storing value of a[i]*a[j]-1 and the left
   index i (with j stored separately in jptr[i]). */
typedef struct {
    seq_t val;   /* a[i] * a[j] - 1 */
    idx_t i;     /* left index */
} Node;

/* -----------  heap of Node values  ----------- */
static Node *heap      = NULL;   /* heap storage */
static size_t heap_size = 0;     /* number of elements currently in heap */
static size_t heap_cap  = 0;     /* capacity of heap storage */

/* swap two nodes */
static inline void heap_swap(Node *x, Node *y) { Node t = *x; *x = *y; *y = t; }

/* push a node onto the heap */
static void heap_push(Node n)
{
    if (heap_size >= heap_cap) {
        heap_cap = heap_cap ? heap_cap << 1 : 4;
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
    }
    size_t pos = heap_size++;
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[parent].val <= n.val) break;
        heap[pos] = heap[parent];
        pos = parent;
    }
    heap[pos] = n;
}

/* pop the smallest node from the heap */
static Node heap_pop(void)
{
    Node res   = heap[0];
    Node last  = heap[--heap_size];
    size_t pos = 0;
    while (1) {
        size_t left = pos * 2 + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t best = left;
        if (right < heap_size && heap[right].val < heap[left].val)
            best = right;
        if (last.val <= heap[best].val) break;
        heap[pos] = heap[best];
        pos = best;
    }
    heap[pos] = last;
    return res;
}

/* -----------  sequence storage  ----------- */
static seq_t *seq   = NULL;        /* array of sequence values (0‑based)   */
static idx_t *jptr  = NULL;        /* next right index for each i          */
static size_t seq_size = 0;        /* number of elements currently stored  */
static size_t seq_cap  = 0;        /* capacity of seq and jptr arrays      */

/* ensure capacity for seq / jptr arrays                         */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t newcap = seq_cap ? seq_cap << 1 : 4;
    while (newcap < need) newcap <<= 1;
    seq  = (seq_t *)realloc(seq,  newcap * sizeof(seq_t));
    jptr = (idx_t *)realloc(jptr, newcap * sizeof(idx_t));
    seq_cap = newcap;
}

/* -----------  required function  ----------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined value, safety */

    /* Lazy initialization – first call */
    if (seq == NULL) {
        /* initialise sequence array with the two seeds */
        seq_cap  = 4;
        seq_size = 2;
        seq   = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        jptr  = (idx_t *)calloc(seq_cap, sizeof(idx_t));

        seq[0] = (seq_t)2;   /* a(1) */
        seq[1] = (seq_t)3;   /* a(2) */

        jptr[0] = 0;         /* start pairing with itself */
        jptr[1] = 1;

        /* initialise heap with self‑pair candidates */
        heap_cap  = 4;
        heap_size = 0;
        heap = (Node *)calloc(heap_cap, sizeof(Node));
        heap_push((Node){ .val = prod_minus_one(seq[0], seq[0]), .i = 0 });
        heap_push((Node){ .val = prod_minus_one(seq[1], seq[1]), .i = 1 });
    }

    /* generate values until we reach the nth element */
    while (seq_size < (size_t)n) {
        Node node = heap_pop();           /* smallest candidate   */
        seq_t cand = node.val;
        idx_t  i   = node.i;

        /* insert only if the value is new (duplicates are consecutive) */
        if (cand != seq[seq_size - 1]) {
            ensure_seq_cap(seq_size + 1);
            seq[seq_size]   = cand;
            jptr[seq_size]  = seq_size;   /* start pairing with itself */
            heap_push((Node){ .val = prod_minus_one(cand, cand), .i = seq_size });
            seq_size++;
        }

        /* advance the right index for i and schedule the next pair */
        jptr[i] = (idx_t)(jptr[i] + 1);
        if (jptr[i] < (idx_t)seq_size) {
            heap_push((Node){ .val = prod_minus_one(seq[i], seq[jptr[i]]), .i = i });
        }
    }

    return seq[n - 1];   /* 1‑based indexing */
}
