/*
 * A020672  Numbers of form x^2 + 9 y^2.
 * https://oeis.org/A020672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 9077, u32 n <= 9600, u64 n <= 9600,
 *            u128 n <= 9600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers of the form x^2 + 9 y^2 (OEIS A020672). */
/* Generates the n-th smallest non‑negative integer representable as x² + 9y². */

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    seq_t val;
    idx_t y;
    idx_t x;
} Node;

/* Insert a node into the min‑heap. */
static inline void heap_insert(Node *heap, idx_t *size, seq_t val, idx_t y, idx_t x)
{
    idx_t i = *size;
    heap[i].val = val;
    heap[i].y   = y;
    heap[i].x   = x;
    (*size)++;

    while (i > 0) {
        idx_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val)
            break;
        Node tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

/* Remove and return the heap minimum. */
static inline Node heap_pop_min(Node *heap, idx_t *size)
{
    Node root = heap[0];
    (*size)--;
    if (*size > 0) {
        heap[0] = heap[*size];
        idx_t i = 0;
        while (1) {
            idx_t l = (i << 1) + 1;
            idx_t r = l + 1;
            if (l >= *size)
                break;
            idx_t s = l;
            if (r < *size && heap[r].val < heap[l].val)
                s = r;
            if (heap[i].val <= heap[s].val)
                break;
            Node tmp = heap[i];
            heap[i] = heap[s];
            heap[s] = tmp;
            i = s;
        }
    }
    return root;
}

seq_t a(idx_t n)
{
    /* Handle trivial case: sequence is indexed from 1. */
    if (n == 0)
        return (seq_t)0;

    /* Heap to merge the sequences x^2 + 9y^2 for each y. */
    Node heap[1024];
    idx_t heap_size = 0;

    /* Initially only y = 0 is known. */
    heap_insert(heap, &heap_size, (seq_t)0, 0, 0);
    idx_t next_y_to_push = 1;

    idx_t produced = 0;
    seq_t last_val = 0;
    bool first = true;

    while (1) {
        Node cur = heap_pop_min(heap, &heap_size);
        seq_t v = cur.val;

        /* Count distinct values only. */
        if (first || v != last_val) {
            produced++;
            if (produced == n)
                return v;
            last_val = v;
            first = false;
        }

        /* Push next value for the same y. */
        idx_t y = cur.y;
        idx_t x_next = cur.x + 1;
        seq_t next_val = ((seq_t)x_next * (seq_t)x_next) +
                         ((seq_t)9 * ((seq_t)y * (seq_t)y));
        heap_insert(heap, &heap_size, next_val, y, x_next);

        /* Add new y sequences whose base value <= current value. */
        while (1) {
            seq_t base = ((seq_t)9 * ((seq_t)next_y_to_push * (seq_t)next_y_to_push));
            if (base > v)
                break;
            heap_insert(heap, &heap_size, base, next_y_to_push, 0);
            next_y_to_push++;
        }
    }
}
