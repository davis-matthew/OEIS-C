/*
 * A123291  Numbers that are sum of a square and a nonnegative cube (with
 *          repetition).
 * https://oeis.org/A123291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth smallest value of i^2 + j^3 (i,j ≥ 0),
   counted with multiplicity. The implementation uses a priority queue
   (binary min‑heap) over the pair (j,i) where j is the cube index and
   i the next square index for that cube. The heap is ordered by the
   value i^2 + j^3. */

/* Note: seq_t may be an arbitrary precision integer, all operations
   (+ - * / %) are supported on seq_t.  idx_t is an unsigned integer
   type (e.g. uint64_t).  No external state is required except the
   standard library. */

#include <stdlib.h>
#include <stdbool.h>

/* Node of the min‑heap: store the cube index j and the current square
   index i.  The value i^2 + j^3 is computed on demand. */
typedef struct {
    idx_t j;   /* cube index (j^3)      */
    idx_t i;   /* next square index    */
} heap_node;

/* Compute the value associated with a heap node. */
static seq_t node_value(const heap_node *n)
{
    seq_t j = (seq_t) n->j;
    j = j * j * j;       /* j^3 */
    seq_t i = (seq_t) n->i;
    i = i * i;           /* i^2 */
    return j + i;
}

/* Comparison used for the heap:
   return -1 if a < b, 0 if equal, 1 if a > b.
   Ties are broken by smaller j. */
static int cmp_heap_node(const heap_node *a, const heap_node *b)
{
    seq_t va = node_value(a);
    seq_t vb = node_value(b);
    if (va < vb) return -1;
    if (va > vb) return 1;
    if (a->j < b->j) return -1;
    if (a->j > b->j) return 1;
    return 0;
}

static void heap_swap(heap_node *a, heap_node *b)
{
    heap_node tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Push‑down from idx keeping the min‑heap property. */
static void heap_sift_down(heap_node *heap, idx_t n, idx_t idx)
{
    for (;;) {
        idx_t left  = (idx << 1) + 1;
        idx_t right = left + 1;
        idx_t smallest = idx;

        if (left < n && cmp_heap_node(&heap[left], &heap[smallest]) < 0)
            smallest = left;
        if (right < n && cmp_heap_node(&heap[right], &heap[smallest]) < 0)
            smallest = right;

        if (smallest == idx)
            break;

        heap_swap(&heap[idx], &heap[smallest]);
        idx = smallest;
    }
}

/* Make idx a root of the heap. */
static void heap_sift_up(heap_node *heap, idx_t idx)
{
    for (;;) {
        if (idx == 0)
            break;
        idx_t parent = (idx - 1) >> 1;
        if (cmp_heap_node(&heap[idx], &heap[parent]) >= 0)
            break;
        heap_swap(&heap[idx], &heap[parent]);
        idx = parent;
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n == 0 we return 0. */
    if (n == 0) return 0;

    /* Use at most n cube indices – safe for n up to 10 000. */
    idx_t jmax     = n;               /* last cube index considered      */
    idx_t heap_size = jmax + 1;       /* heap holds entries 0..jmax       */

    heap_node *heap = (heap_node*)malloc(heap_size * sizeof(heap_node));
    if (!heap) return 0;              /* allocation failure: safe fallback */

    /* Initialise heap: each cube j starting with i = 0. */
    for (idx_t j = 0; j <= jmax; ++j) {
        heap[j].j = j;
        heap[j].i = 0;
    }

    /* Build the min‑heap. */
    if (heap_size > 1) {
        for (idx_t i = heap_size / 2; i-- > 0; )
            heap_sift_down(heap, heap_size, i);
    }

    /* Extract the n‑th smallest value. */
    for (idx_t k = 1; ; ++k) {
        heap_node cur = heap[0];
        seq_t cur_sum = node_value(&cur);

        if (k == n) {
            free(heap);
            return cur_sum;
        }

        /* Advance to next square for this cube index j. */
        cur.i++;
        heap[0] = cur;
        heap_sift_down(heap, heap_size, 0);
    }
}
