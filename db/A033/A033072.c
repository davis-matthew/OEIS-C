/*
 * A033072  Numbers with the property that all pairs of consecutive base-7
 *          digits differ by 1.
 * https://oeis.org/A033072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth smallest positive integer whose base‑7 digits
 *            differ by 1
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Minimal binary heap for seq_t */
struct Heap {
    seq_t *data;
    size_t size;
    size_t cap;
};

static void heap_push(struct Heap *h, seq_t v)
{
    if (h->size == h->cap) {
        size_t newcap = h->cap? h->cap*2 : 8;
        h->data = (seq_t *)realloc(h->data, newcap * sizeof(seq_t));
        h->cap = newcap;
    }
    size_t i = h->size++;
    h->data[i] = v;
    while (i > 0) {
        size_t parent = (i-1)/2;
        if (h->data[parent] <= h->data[i]) break;
        seq_t tmp = h->data[parent];
        h->data[parent] = h->data[i];
        h->data[i] = tmp;
        i = parent;
    }
}

static seq_t heap_pop(struct Heap *h)
{
    seq_t min = h->data[0];
    h->size--;
    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        size_t i = 0;
        while (1) {
            size_t left = 2*i + 1;
            size_t right = left + 1;
            size_t smallest = i;
            if (left < h->size && h->data[left] < h->data[smallest]) smallest = left;
            if (right < h->size && h->data[right] < h->data[smallest]) smallest = right;
            if (smallest == i) break;
            seq_t tmp = h->data[i];
            h->data[i] = h->data[smallest];
            h->data[smallest] = tmp;
            i = smallest;
        }
    }
    return min;
}

/* Append a value to a dynamic array */
static void seq_append(seq_t **arr, size_t *size, size_t *cap, seq_t val)
{
    if (*size == *cap) {
        size_t newcap = *cap? *cap*2 : 8;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *seq = NULL;
    static size_t seqsize = 0, seqcap = 0;
    static struct Heap heap = {NULL,0,0};
    static int initialized = 0;

    /* initialize on first call */
    if (!initialized) {
        seqcap = 16;
        seq = (seq_t *)malloc(seqcap * sizeof(seq_t));

        heap.cap = 16;
        heap.data = (seq_t *)malloc(heap.cap * sizeof(seq_t));

        /* push the single‑digit numbers 1 … 6 */
        for (idx_t i = 1; i <= 6; ++i) {
            heap_push(&heap, (seq_t)i);
        }
        initialized = 1;
    }

    /* Generate terms until we have at least n of them */
    while (seqsize < (size_t)n) {
        seq_t val = heap_pop(&heap);

        /* store the new term */
        seq_append(&seq, &seqsize, &seqcap, val);

        /* generate its children (next digits differ by 1) */
        seq_t last = val % (seq_t)7;
        if (last > 0) {
            seq_t child = val * (seq_t)7 + (last - 1);
            heap_push(&heap, child);
        }
        if (last < 6) {
            seq_t child = val * (seq_t)7 + (last + 1);
            heap_push(&heap, child);
        }
    }

    return seq[n-1];
}
