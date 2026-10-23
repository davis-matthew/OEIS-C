/*
 * A033829  Numbers that can be expressed as the product of two 3-digit
 *          numbers in at least one way.
 * https://oeis.org/A033829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A033829:
   the smallest distinct products of two 3‑digit numbers (100..999),
   sorted increasingly.  The sequence starts with 10000, 10100, 10200, … */

/*  Requires <stdint.h> and <stdbool.h> for the integer and bool types. */
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    seq_t prod;       /* product i * j */
    uint32_t i;       /* i in [100,999]  */
    uint32_t j;       /* j in [i,999]    */
} Node;

/* push a node onto a binary min‑heap */
static inline void heap_push(Node heap[], idx_t *size, Node v)
{
    idx_t pos = (++(*size));
    while (pos > 1) {
        idx_t parent = pos >> 1;
        if (heap[parent].prod <= v.prod) break;
        heap[pos] = heap[parent];
        pos = parent;
    }
    heap[pos] = v;
}

/* pop the smallest node from a binary min‑heap */
static inline Node heap_pop(Node heap[], idx_t *size)
{
    Node min = heap[1];
    Node last = heap[(*size)--];
    idx_t pos = 1;
    while (1) {
        idx_t left = pos << 1;
        if (left > *size) break;
        idx_t right = left + 1;
        idx_t child = (right <= *size && heap[right].prod < heap[left].prod) ? right : left;
        if (last.prod <= heap[child].prod) break;
        heap[pos] = heap[child];
        pos = child;
    }
    heap[pos] = last;
    return min;
}

seq_t a(idx_t n)
{
    /* The heap can hold at most 900 elements (one per i). */
    Node heap[901];
    idx_t heap_size = 0;
    /* Initialize the heap with (i,i) pairs. */
    for (uint32_t i = 100; i <= 999; ++i) {
        Node v;
        v.i = i;
        v.j = i;
        v.prod = (seq_t)i * (seq_t)i;
        heap_push(heap, &heap_size, v);
    }

    seq_t last_val = 0;
    bool last_set = false;
    idx_t count = 0;

    while (1) {
        Node cur = heap_pop(heap, &heap_size);

        /* Record the value only if it differs from the previous one. */
        if (!last_set || cur.prod != last_val) {
            ++count;
            if (count == n)
                return cur.prod;
            last_val = cur.prod;
            last_set = true;
        }

        /* Advance to the next j for the same i, if any. */
        if (cur.j < 999) {
            Node nxt;
            nxt.i = cur.i;
            nxt.j = cur.j + 1;
            /* prod(i, j+1) = prod(i,j) + i */
            nxt.prod = cur.prod + (seq_t)cur.i;
            heap_push(heap, &heap_size, nxt);
        }
    }
}
