/*
 * A046083  The smallest member 'a' of the Pythagorean triples (a,b,c) ordered
 *          by increasing c.
 * https://oeis.org/A046083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the smallest member 'a' of all integer Pythagorean triples,
   ordered by increasing hypotenuse c, and for equal c by decreasing a. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd_int.h"

typedef struct {
    seq_t c;          /* hypotenuse of the triple */
    seq_t a;          /* smallest leg of the triple */
    seq_t base_c;     /* hypotenuse of the primitive triple */
    seq_t base_a;     /* smallest leg of the primitive triple */
    unsigned int k;   /* current multiplier */
} Node;

static seq_t *cache     = NULL;   /* cached a(n) values */
static idx_t  cache_sz  = 0;
static size_t cache_cap = 0;

static Node *heap      = NULL;    /* min‑heap of Nodes */
static size_t heap_sz  = 0;
static size_t heap_cap = 0;
static bool   heap_built = false;

static inline bool node_higher(const Node *x, const Node *y)
{
    /* true if x should be above y in the min‑heap */
    if (x->c < y->c) return true;
    if (x->c > y->c) return false;
    return x->a > y->a;  /* tie: larger a first (descending a) */
}

static void heap_push(Node node)
{
    if (heap_sz == heap_cap) {
        size_t new_cap = (heap_cap == 0) ? 16 : heap_cap << 1;
        Node *new_arr = (Node *)realloc(heap, new_cap * sizeof(Node));
        if (!new_arr) abort();
        heap = new_arr;
        heap_cap = new_cap;
    }
    heap[heap_sz] = node;
    size_t i = heap_sz++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (node_higher(&heap[i], &heap[p])) {
            Node tmp = heap[i];
            heap[i] = heap[p];
            heap[p] = tmp;
            i = p;
        } else break;
    }
}

static Node heap_pop(void)
{
    Node top = heap[0];
    --heap_sz;
    if (heap_sz > 0) {
        heap[0] = heap[heap_sz];
        size_t i = 0;
        while (1) {
            size_t l = (i << 1) + 1;
            if (l >= heap_sz) break;
            size_t r = l + 1;
            size_t c = (r < heap_sz && node_higher(&heap[r], &heap[l])) ? r : l;
            if (node_higher(&heap[c], &heap[i])) {
                Node tmp = heap[i];
                heap[i] = heap[c];
                heap[c] = tmp;
                i = c;
            } else break;
        }
    }
    return top;
}

static void build_primitives_and_heap(void)
{
    if (heap_built) return;
    const idx_t MAX_C_LIMIT = 200000;   /* sufficient for n≈10000 */
    for (idx_t m = 2; m * m <= MAX_C_LIMIT; ++m) {
        for (idx_t n = 1; n < m; ++n) {
            if (((m - n) & 1) && gcd_int((int)m, (int)n) == 1) {
                seq_t Mm = (seq_t)m;
                seq_t Nm = (seq_t)n;
                seq_t a0 = Mm * Mm - Nm * Nm;
                seq_t b0 = (seq_t)2 * Mm * Nm;
                seq_t small = (a0 < b0) ? a0 : b0;
                seq_t c0 = Mm * Mm + Nm * Nm;
                Node node = { .c = c0, .a = small, .base_c = c0,
                              .base_a = small, .k = 1 };
                heap_push(node);
            }
        }
    }
    heap_built = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (!cache) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    if (!heap_built) build_primitives_and_heap();

    if ((idx_t)cache_sz < n) {
        if ((idx_t)cache_cap < n) {
            size_t new_cap = cache_cap;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_arr) abort();
            cache = new_arr;
            cache_cap = new_cap;
        }
        while ((idx_t)cache_sz < n) {
            Node node = heap_pop();
            cache[cache_sz++] = node.a;
            unsigned int next_k = node.k + 1;
            Node next;
            next.k = next_k;
            next.base_c = node.base_c;
            next.base_a = node.base_a;
            next.c = node.base_c * (seq_t)next_k;
            next.a = node.base_a * (seq_t)next_k;
            heap_push(next);
        }
    }

    return cache[n - 1];
}
