/*
 * A094564  Triple products of distinct Fibonacci numbers: F(i)F(j)F(k),
 *          2<=i<j<k.
 * https://oeis.org/A094564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   encode, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS A094564:
   a(n) = the n-th smallest product F(i)*F(j)*F(k) with 2 ≤ i < j < k,
   where F(m) is the m-th Fibonacci number. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Fibonacci helper (provided by the build system). */
#include "../utils/fib.h"
#include "../utils/encode.h"

/* heap node ---------------------------------------------------------- */
typedef struct {
    idx_t i, j, k;   /* indices, 2 ≤ i < j < k  */
    seq_t prod;      /* product F(i)*F(j)*F(k) */
} Node;

/* simple min‑heap of Node -------------------------------------------- */
static Node *heap   = NULL;
static size_t heap_size = 0;
static size_t heap_cap  = 0;

static inline int cmpNode(const Node *a, const Node *b)
{
    if (a->prod < b->prod) return -1;
    if (a->prod > b->prod) return  1;
    return 0;
}

static void heap_init(void)
{
    heap_cap = 64;
    heap = (Node *)malloc(heap_cap * sizeof(Node));
    heap_size = 0;
}

static void heap_push(Node v)
{
    if (!heap) heap_init();
    if (heap_size == heap_cap) {
        heap_cap <<= 1;
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
    }
    size_t pos = heap_size++;
    while (pos && cmpNode(&v, &heap[(pos-1)>>1]) < 0) {
        heap[pos] = heap[(pos-1)>>1];
        pos = (pos-1)>>1;
    }
    heap[pos] = v;
}

static Node heap_pop(void)
{
    Node top = heap[0];
    Node last = heap[--heap_size];
    size_t pos = 0;
    while (1) {
        size_t left = (pos<<1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t best = left;
        if (right < heap_size && cmpNode(&heap[right], &heap[left]) < 0)
            best = right;
        if (cmpNode(&last, &heap[best]) <= 0) break;
        heap[pos] = heap[best];
        pos = best;
    }
    heap[pos] = last;
    return top;
}

/* visited set ------------------------------------------- */
/* stores encoded triples, simple open addressing              */
static uint64_t *vtable = NULL;
static size_t vt_cap   = 0;
static size_t vt_mask  = 0;
#define VISIT_EMPTY ((uint64_t)-1)

static void vt_init(void)
{
    vt_cap  = 1 << 16;          /* 65536 slots, * 2 = 131072 < 2^20            */
    vt_mask = vt_cap - 1;
    vtable  = (uint64_t *)malloc(vt_cap * sizeof(uint64_t));
    for (size_t t = 0; t < vt_cap; ++t) vtable[t] = VISIT_EMPTY;
}

static bool vt_insert(uint64_t key)
{
    size_t pos = key & vt_mask;
    while (vtable[pos] != VISIT_EMPTY) {
        if (vtable[pos] == key) return false;   /* already stored */
        pos = (pos + 1) & vt_mask;
    }
    vtable[pos] = key;
    return true;
}

/* sequence cache ------------------------------------------------------ */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

static void seq_init(void)
{
    seq_cap   = 64;
    seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_len   = 0;
}

/* global state -------------------------------------------------------- */
static seq_t last_output;
static bool has_last = false;

/* generate terms up to index n ---------------------------------------- */
static void generate_upto(idx_t n)
{
    if (seq_len >= n) return;

    /* initialise structures once */
    if (!heap)    heap_init();
    if (!vtable) vt_init();
    if (!seq_cache) seq_init();

    /* push the first triple (2,3,4) only once */
    if (seq_len == 0) {
        Node first = {2,3,4, fib(2)*fib(3)*fib(4)};
        heap_push(first);
        vt_insert(encode(first.i,first.j,first.k));
    }

    while (seq_len < n) {
        Node cur = heap_pop();

        /* store unique product */
        if (!has_last || cur.prod != last_output) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = cur.prod;
            last_output = cur.prod;
            has_last   = true;
        }

        /* generate successors */

        /* (i+1, j, k)   allowed if i+1 < j */
        if (cur.i + 1 < cur.j) {
            if (vt_insert(encode(cur.i+1, cur.j, cur.k))) {
                Node nnode = {cur.i+1, cur.j, cur.k,
                              fib(cur.i+1)*fib(cur.j)*fib(cur.k)};
                heap_push(nnode);
            }
        }

        /* (i, j+1, k)   allowed if j+1 < k */
        if (cur.j + 1 < cur.k) {
            if (vt_insert(encode(cur.i, cur.j+1, cur.k))) {
                Node nnode = {cur.i, cur.j+1, cur.k,
                              fib(cur.i)*fib(cur.j+1)*fib(cur.k)};
                heap_push(nnode);
            }
        }

        /* (i, j, k+1)   always allowed (k grows) */
        if (vt_insert(encode(cur.i, cur.j, cur.k+1))) {
            Node nnode = {cur.i, cur.j, cur.k+1,
                          fib(cur.i)*fib(cur.j)*fib(cur.k+1)};
            heap_push(nnode);
        }
    }
}

/* public API ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */
    generate_upto(n);
    return seq_cache[n-1];
}
