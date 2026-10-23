/*
 * A186129  Numbers that can be partitioned into four parts s, t, u, v such
 *          that s+k = t-k = u*k = v/k for some k > 1.
 * https://oeis.org/A186129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1178, u64 n <= 1178, u128 n <= 1178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186129
   --
   a(n) is the n-th smallest integer that can be written as
   a = k * m^2  with integers k >= 2 and m >= 3.
   Equivalently, a = k*(b+1)^2 where b = m-1 >= 2.
   The list is sorted increasingly and duplicates are omitted.
   The algorithm generates the values by merging the arithmetic
   progressions for each fixed m, using a min‑heap.  The state is
   kept static so that calls do not have to be in increasing order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- node type for the heap ---------- */
typedef struct {
    seq_t   val;   /* current value k*m^2 */
    idx_t   m;     /* fixed base (m >= 3) */
    idx_t   k;     /* current multiplier (k >= 2) */
} heap_node;

/* ---------- simple binary min‑heap operations ---------- */
static void heap_swap(heap_node *a, heap_node *b) {
    heap_node tmp = *a; *a = *b; *b = tmp;
}

static void heap_push(heap_node **heap, size_t *hs, size_t *hc, heap_node node) {
    if (*hs == *hc) {
        *hc = (*hc == 0) ? 4 : (*hc << 1);
        *heap = (heap_node *)realloc(*heap, *hc * sizeof(heap_node));
    }
    size_t i = (*hs)++;
    (*heap)[i] = node;
    /* sift up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if ((*heap)[p].val <= (*heap)[i].val) break;
        heap_swap(&(*heap)[p], &(*heap)[i]);
        i = p;
    }
}

static heap_node heap_pop(heap_node *heap, size_t *hs) {
    heap_node res = heap[0];
    heap[0] = heap[--(*hs)];
    /* sift down */
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= *hs) break;
        size_t r = l + 1;
        size_t s = (r < *hs && heap[r].val < heap[l].val) ? r : l;
        if (heap[i].val <= heap[s].val) break;
        heap_swap(&heap[i], &heap[s]);
        i = s;
    }
    return res;
}

/* ---------- sequence implementation ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* undefined case, return 0 */

    /* static state for lazy initialization and memoization */
    static bool      inited    = false;
    static seq_t    *seq       = NULL;   /* stored sequence values */
    static size_t    seq_len   = 0;      /* number of generated terms   */
    static size_t    seq_cap   = 0;      /* capacity of seq array       */

    static heap_node *heap     = NULL;   /* min‑heap of active nodes    */
    static size_t     heap_len = 0;      /* current heap size           */
    static size_t     heap_cap = 0;      /* current heap capacity       */

    const idx_t MAX_M = 2000;            /* enough for n up to 10 000  */

    /* ---------- initialization ---------- */
    if (!inited) {
        /* max capacity for the sequence array */
        seq_cap = 256;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));

        /* initialise heap with the first value for each m >= 3 */
        heap_cap = (size_t)(MAX_M - 2);
        heap     = (heap_node *)malloc(heap_cap * sizeof(heap_node));
        heap_len = 0;
        for (idx_t m = 3; m <= MAX_M; ++m) {
            heap_node node;
            node.m = m;
            node.k = 2; /* first multiplier */
            node.val = (seq_t)node.m * (seq_t)node.m * (seq_t)node.k;
            heap_push(&heap, &heap_len, &heap_cap, node);
        }

        inited = true;
    }

    /* ---------- generate terms until we have at least n of them ---------- */
    while (seq_len < (size_t)n) {
        /* get the smallest available value */
        heap_node cur = heap_pop(heap, &heap_len);

        /* store it if not a duplicate of the previous one */
        if (seq_len == 0 || cur.val != seq[seq_len - 1]) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;                      /* double capacity */
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = cur.val;
        }

        /* push the next multiple for the same m */
        cur.k += 1;
        cur.val = (seq_t)cur.m * (seq_t)cur.m * (seq_t)cur.k;
        heap_push(&heap, &heap_len, &heap_cap, cur);
    }

    return seq[n - 1];   /* 0‑based indexing in the stored array */
}
