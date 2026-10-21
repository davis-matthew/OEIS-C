/*
 * A052499  If n is in the sequence then so are 2n and 4n-1.
 * https://oeis.org/A052499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   generate the sequence by the closure rules * a(0) = 1; if n is
 *            in the sequence then 2n and 4n‑1 are also * in the sequence. The
 *            algorithm extracts terms from a * min‑heap to output them in
 *            increasing order
 * Range:     u8 n <= 34, u16 n <= 1868, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Min‑heap for seq_t values. */
typedef struct {
    seq_t *data;   /* heap array */
    size_t size;   /* number of elements in the heap */
    size_t cap;    /* allocated capacity */
} Heap;

/* Ensure that the heap can hold at least min_cap elements. */
static void heap_ensure_cap(Heap *h, size_t min_cap)
{
    if (h->cap >= min_cap) return;
    size_t new_cap = h->cap ? h->cap * 2 : 4;
    while (new_cap < min_cap) new_cap <<= 1;
    h->data = (seq_t*)realloc(h->data, new_cap * sizeof(seq_t));
    h->cap = new_cap;
}

/* Restore heap order by moving an element up. */
static void heap_sift_up(Heap *h, size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (h->data[parent] <= h->data[idx]) break;
        seq_t tmp = h->data[parent];
        h->data[parent] = h->data[idx];
        h->data[idx] = tmp;
        idx = parent;
    }
}

/* Restore heap order by moving an element down. */
static void heap_sift_down(Heap *h, size_t idx)
{
    size_t sz = h->size;
    while (1) {
        size_t left = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;
        if (left < sz && h->data[left] < h->data[smallest]) smallest = left;
        if (right < sz && h->data[right] < h->data[smallest]) smallest = right;
        if (smallest == idx) break;
        seq_t tmp = h->data[smallest];
        h->data[smallest] = h->data[idx];
        h->data[idx] = tmp;
        idx = smallest;
    }
}

/* Insert a value into the heap. */
static void heap_push(Heap *h, seq_t v)
{
    heap_ensure_cap(h, h->size + 1);
    h->data[h->size] = v;
    heap_sift_up(h, h->size);
    ++h->size;
}

/* Remove and return the smallest value from the heap. */
static seq_t heap_pop(Heap *h)
{
    seq_t top = h->data[0];
    --h->size;
    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        heap_sift_down(h, 0);
    }
    return top;
}

/* Global state for memoised values. */
static seq_t *seq_arr = NULL;
static size_t seq_cnt = 0;   /* number of terms currently stored */
static size_t seq_cap = 0;   /* allocated capacity of seq_arr */
static Heap seq_heap = { NULL, 0, 0 };

/* Ensure that terms up to index n are pre‑computed. */
static void ensure_precomputed(idx_t n)
{
    if (seq_cnt > (size_t)n) return;

    /* Initialise on first call. */
    if (!seq_heap.data) {
        heap_ensure_cap(&seq_heap, 4);
        heap_push(&seq_heap, (seq_t)1);          /* seed value a(0)=1 */
        seq_cap = 4;
        seq_arr = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_cnt = 0;
    }

    /* Grow the result array as needed. */
    while (seq_cnt <= (size_t)n) {
        seq_t val = heap_pop(&seq_heap);

        if (seq_cnt >= seq_cap) {
            seq_cap <<= 1;
            seq_arr = (seq_t*)realloc(seq_arr, seq_cap * sizeof(seq_t));
        }
        seq_arr[seq_cnt++] = val;

        /* Generate successors 2*val and 4*val-1. */
        seq_t twice = val * (seq_t)2;
        seq_t four_minus_one = (val * (seq_t)4) - (seq_t)1;
        heap_push(&seq_heap, twice);
        heap_push(&seq_heap, four_minus_one);
    }
}

/* Public function: return the nth term of A052499. */
seq_t a(idx_t n)
{
    ensure_precomputed(n);
    return seq_arr[(size_t)n];
}
