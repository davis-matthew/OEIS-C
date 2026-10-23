/*
 * A045619  Numbers that are the products of 2 or more consecutive integers.
 * https://oeis.org/A045619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = numbers that are the products of 2 or more consecutive
 *            integers (computed by a priority‑queue based search)
 * Range:     u8 n <= 4, u16 n <= 312, u32 n <= 2322, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Node encodes a running product of consecutive integers
   starting at k, current length l (>=2), and the product itself.
*/
typedef struct {
    seq_t prod;   /* current product */
    idx_t k;      /* first integer in the run   */
    idx_t l;      /* current length of the run  */
} Node;

/* Simple binary min‑heap for Nodes, ordered by prod. */
typedef struct {
    Node *data;
    size_t size;
    size_t cap;
} Heap;

static void heap_init(Heap *h, size_t cap) {
    h->data = (Node *)malloc(cap * sizeof(Node));
    h->size = 0;
    h->cap  = cap;
}

static void heap_push(Heap *h, Node node) {
    if (h->size == h->cap) {
        h->cap *= 2;
        h->data = (Node *)realloc(h->data, h->cap * sizeof(Node));
    }
    size_t i = h->size++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h->data[p].prod <= node.prod) break;
        h->data[i] = h->data[p];
        i = p;
    }
    h->data[i] = node;
}

static Node heap_pop(Heap *h) {
    Node ret = h->data[0];
    Node last = h->data[--h->size];
    size_t i = 0;
    while (true) {
        size_t l = 2 * i + 1;
        if (l >= h->size) break;
        size_t r = l + 1;
        size_t j = (r < h->size && h->data[r].prod < h->data[l].prod) ? r : l;
        if (last.prod <= h->data[j].prod) break;
        h->data[i] = h->data[j];
        i = j;
    }
    h->data[i] = last;
    return ret;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;   /* a(1) = 0 */

    /* number of terms to generate after the initial 0 */
    idx_t need = n - 1;
    /* initial heap capacity – enough to cover the required range */
    size_t init_cap = need * 2 + 10;

    Heap h;
    heap_init(&h, init_cap);

    /* Insert starting products k*(k+1) for k = 1 .. init_cap */
    for (idx_t k = 1; k <= init_cap; ++k) {
        Node n;
        n.prod = (seq_t)k * ((seq_t)k + 1);
        n.k    = k;
        n.l    = 2;
        heap_push(&h, n);
    }

    seq_t last = 0;
    bool last_set = false;
    seq_t result = 0;
    idx_t count = 0;

    while (count < need) {
        /* Extract smallest product */
        Node cur = heap_pop(&h);

        /* Skip duplicates */
        if (!last_set || cur.prod != last) {
            last = cur.prod;
            last_set = true;
            ++count;
            if (count == need) {
                result = cur.prod;
                break;
            }
        }

        /* Extend the run by one more consecutive integer */
        Node nxt;
        nxt.k = cur.k;
        nxt.l = cur.l + 1;
        nxt.prod = cur.prod * ((seq_t)(cur.k + cur.l));
        heap_push(&h, nxt);
    }

    free(h.data);
    return result;
}
