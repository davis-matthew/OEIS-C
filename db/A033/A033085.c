/*
 * A033085  Numbers in which all pairs of consecutive base-7 digits differ by
 *          2.
 * https://oeis.org/A033085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑7 digits differ by exactly 2. */
/* The algorithm generates all admissible numbers in increasing order by
   maintaining a min‑heap of partial numbers. Each node stores the current
   value and its last base‑7 digit.  When a node is popped, its value is
   the next term of the sequence and children corresponding to
   (last_digit ± 2) are pushed.  The sequence is memoised in a growing
   array so subsequent calls are O(1) for already‑computed terms. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

struct HeapNode {
    seq_t val;
    uint8_t last;
};

static struct HeapNode *heap = NULL;
static size_t heap_size = 0, heap_cap = 0;

static seq_t *seq = NULL;
static size_t seq_len = 0, seq_cap = 0;

static void heap_swap(size_t i, size_t j)
{
    struct HeapNode tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

static void heap_push(seq_t v, uint8_t d)
{
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 16;
        heap = (struct HeapNode *)realloc(heap, heap_cap * sizeof(struct HeapNode));
    }
    size_t i = heap_size++;
    heap[i].val = v;
    heap[i].last = d;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[i].val < heap[parent].val) {
            heap_swap(i, parent);
            i = parent;
        } else {
            break;
        }
    }
}

static struct HeapNode heap_pop(void)
{
    struct HeapNode top = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t left = i * 2 + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_size && heap[right].val < heap[left].val)
            smallest = right;
        if (heap[smallest].val < heap[i].val) {
            heap_swap(i, smallest);
            i = smallest;
        } else {
            break;
        }
    }
    return top;
}

static void seq_append(seq_t v)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 64;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_len++] = v;
}

seq_t a(idx_t n)
{
    /* initialise the heap with one‑digit numbers 1..6 (if not yet done) */
    if (!heap) {
        heap_cap = 16;
        heap = (struct HeapNode *)malloc(heap_cap * sizeof(struct HeapNode));
        for (int d = 1; d <= 6; ++d)
            heap_push((seq_t)d, (uint8_t)d);
    }

    /* extend the stored sequence until it contains at least n terms */
    while (seq_len < n) {
        struct HeapNode cur = heap_pop();
        seq_append(cur.val);

        int d = cur.last;
        int d1 = d - 2;
        if (d1 >= 0) {
            seq_t new_val = cur.val * (seq_t)7 + (seq_t)d1;
            heap_push(new_val, (uint8_t)d1);
        }
        int d2 = d + 2;
        if (d2 <= 6) {
            seq_t new_val = cur.val * (seq_t)7 + (seq_t)d2;
            heap_push(new_val, (uint8_t)d2);
        }
    }
    return seq[n - 1];
}
