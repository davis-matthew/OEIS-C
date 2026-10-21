/*
 * A055600  Numbers of form 2^i*3^j + 1 with i, j >= 0.
 * https://oeis.org/A055600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 93, u32 n <= 337, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bs_find, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate numbers 2^i*3^j + 1 in ascending order
 * using a min‑heap and a sorted set of seen values. */
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../utils/mul_small.h"
#include "../utils/bs_find.h"

typedef struct {
    seq_t  val; /* 2^i * 3^j */
    idx_t  i;   /* exponent of 2   */
    idx_t  j;   /* exponent of 3   */
} node;

/* push a node onto a min‑heap */
static void heap_push(node *heap, size_t *hs, size_t *hc, node n)
{
    if (*hs == *hc) {                     /* grow the heap array if needed */
        *hc = (*hc * 2) + 1;
        heap = (node*)realloc(heap, *hc * sizeof(node));
    }
    heap[*hs] = n;
    size_t pos = *hs;
    ++*hs;

    /* sift up */
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[pos].val < heap[parent].val) {
            node tmp = heap[pos];
            heap[pos] = heap[parent];
            heap[parent] = tmp;
            pos = parent;
        } else {
            break;
        }
    }
}

/* pop the smallest node from the min‑heap */
static node heap_pop(node *heap, size_t *hs)
{
    node top = heap[0];
    --*hs;
    heap[0] = heap[*hs];

    /* sift down */
    size_t pos = 0;
    for (;;) {
        size_t left  = (pos << 1) + 1;
        size_t right = left + 1;
        size_t smallest = pos;
        if (left < *hs && heap[left].val < heap[smallest].val)
            smallest = left;
        if (right < *hs && heap[right].val < heap[smallest].val)
            smallest = right;
        if (smallest == pos)
            break;
        node tmp = heap[pos];
        heap[pos] = heap[smallest];
        heap[smallest] = tmp;
        pos = smallest;
    }
    return top;
}

/* ----------------------------------------------------------------
 * Public API function
 * ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset starts at 1, but return 0 for safety */

    /* static state across calls */
    static seq_t *seq      = NULL;    /* array of a(k) values           */
    static size_t seq_len  = 0;
    static size_t seq_cap  = 0;

    static node *heap      = NULL;
    static size_t heap_sz  = 0;
    static size_t heap_cap = 0;

    static seq_t *seen     = NULL;
    static size_t seen_len = 0;
    static size_t seen_cap = 0;

    static seq_t last_val  = 0;
    static int   have_last = 0;        /* flag: has last_val been set?  */

    /* initialise on first call */
    if (!seq) {
        seq_cap = 128;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));

        heap_cap = 128;
        heap = (node*)malloc(heap_cap * sizeof(node));

        seen_cap = 128;
        seen = (seq_t*)malloc(seen_cap * sizeof(seq_t));

        /* push the initial product 1 (=2^0*3^0) */
        node root = { .val = 1, .i = 0, .j = 0 };
        heap_push(heap, &heap_sz, &heap_cap, root);

        /* mark 1 as seen */
        seen[0] = 1;
        seen_len = 1;
    }

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        node cur = heap_pop(heap, &heap_sz);

        /* if this value is new (different from the previous popped) */
        if (!have_last || cur.val != last_val) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = cur.val + 1;   /* add 1 as required by the sequence */
            last_val = cur.val;
            have_last = 1;
        }

        /* generate children: multiply by 2 and by 3 */
        node child1, child2;
        child1.val = mul_small(cur.val, 2);
        child1.i   = cur.i + 1;
        child1.j   = cur.j;

        child2.val = mul_small(cur.val, 3);
        child2.i   = cur.i;
        child2.j   = cur.j + 1;

        /* push child1 if it hasn't been seen before */
        size_t idx = bs_find(seen, seen_len, child1.val);
        if (idx == seen_len || seen[idx] != child1.val) {
            if (seen_len == seen_cap) {
                seen_cap *= 2;
                seen = (seq_t*)realloc(seen, seen_cap * sizeof(seq_t));
            }
            /* make room for the new value */
            if (seen_len > idx)
                memmove(&seen[idx + 1], &seen[idx], (seen_len - idx) * sizeof(seq_t));
            seen[idx] = child1.val;
            seen_len++;

            heap_push(heap, &heap_sz, &heap_cap, child1);
        }

        /* push child2 if it hasn't been seen before */
        idx = bs_find(seen, seen_len, child2.val);
        if (idx == seen_len || seen[idx] != child2.val) {
            if (seen_len == seen_cap) {
                seen_cap *= 2;
                seen = (seq_t*)realloc(seen, seen_cap * sizeof(seq_t));
            }
            if (seen_len > idx)
                memmove(&seen[idx + 1], &seen[idx], (seen_len - idx) * sizeof(seq_t));
            seen[idx] = child2.val;
            seen_len++;

            heap_push(heap, &heap_sz, &heap_cap, child2);
        }
    }

    return seq[n - 1];
}
