/*
 * A183987  Ranks of (odd i)+j*r, when all i+j*r are ranked; r=golden ratio
 *          (1+sqrt(5))/2, i>=0, j>=0. Complement of A183988.
 * https://oeis.org/A183987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the rank of the nth pair (i,j) with odd i in the ordering
 *            of i + j*φ, where φ = (1+√5)/2. The ordering is the natural
 *            ordering of the real numbers i + j*φ for i,j ≥ 0. The term a(n)
 *            is returned for n ≥ 1
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
 
/* High‑precision constant for the golden ratio (1+√5)/2. */
static const long double GOLDEN_RATIO =
    1.6180339887498948482045868343656381177203091798057580023L;
 
/* Entry in the priority queue. */
typedef struct {
    idx_t i;          /* i component of the pair */
    idx_t j;          /* j component of the pair */
    long double val;  /* i + j*GOLDEN_RATIO */
} Entry;
 
/* Simple binary min heap. */
typedef struct {
    Entry *data;      /* heap array (0‑based) */
    size_t size;      /* number of elements in heap */
    size_t cap;       /* current capacity of heap array */
} Heap;
 
static void heap_init(Heap *h)
{
    h->size = 0;
    h->cap  = 64;
    h->data = (Entry *)malloc(h->cap * sizeof(Entry));
}
 
static void heap_free(Heap *h)
{
    free(h->data);
    h->size = h->cap = 0;
}
 
static void heap_push(Heap *h, idx_t i, idx_t j, long double val)
{
    if (h->size == h->cap) {
        h->cap *= 2;
        h->data = (Entry *)realloc(h->data, h->cap * sizeof(Entry));
    }
    size_t pos = h->size++;
    h->data[pos].i   = i;
    h->data[pos].j   = j;
    h->data[pos].val = val;
    /* sift‑up */
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (h->data[pos].val < h->data[parent].val) {
            Entry tmp = h->data[pos];
            h->data[pos] = h->data[parent];
            h->data[parent] = tmp;
            pos = parent;
        } else {
            break;
        }
    }
}
 
static Entry heap_pop(Heap *h)
{
    Entry top = h->data[0];
    h->data[0] = h->data[--h->size];
    size_t pos = 0;
    /* sift‑down */
    while (1) {
        size_t left  = (pos << 1) + 1;
        size_t right = left + 1;
        size_t smallest = pos;
        if (left < h->size && h->data[left].val < h->data[smallest].val)
            smallest = left;
        if (right < h->size && h->data[right].val < h->data[smallest].val)
            smallest = right;
        if (smallest == pos)
            break;
        Entry tmp = h->data[pos];
        h->data[pos] = h->data[smallest];
        h->data[smallest] = tmp;
        pos = smallest;
    }
    return top;
}
 
/* Main sequence generation function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1, return 0 for safety */
 
    Heap heap;
    heap_init(&heap);
 
    /* Insert the first pair (i=0, j=0). */
    heap_push(&heap, (idx_t)0, (idx_t)0, (long double)0.0L);
 
    idx_t next_i_to_add = 1;   /* next i to be added as (i,0) */
    idx_t rank          = 0;   /* current rank in the global ordering */
    idx_t odd_cnt       = 0;   /* number of odd-i pairs seen so far */
 
    while (odd_cnt < n) {
        Entry e = heap_pop(&heap);
        ++rank;
 
        if (e.i & 1) {  /* i is odd */
            ++odd_cnt;
            if (odd_cnt == n) {
                heap_free(&heap);
                return (seq_t)rank;
            }
        }
 
        /* Insert next j for the same i. */
        idx_t new_j = e.j + 1;
        long double new_val = (long double)e.i + ((long double)new_j) * GOLDEN_RATIO;
        heap_push(&heap, e.i, new_j, new_val);
 
        /* Insert next new i with j = 0. */
        if (next_i_to_add < (idx_t)(-1)) {   /* avoid overflow */
            long double val_i0 = (long double)next_i_to_add; /* j = 0 */
            heap_push(&heap, next_i_to_add, (idx_t)0, val_i0);
            ++next_i_to_add;
        }
    }
 
    /* Should never reach here */
    heap_free(&heap);
    return (seq_t)0;
}
