/*
 * A085783  Product of a prime and a triangular number.
 * https://oeis.org/A085783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th distinct product of a prime and a triangular
 *            number, sorted increasingly (a(1)=2)
 * Range:     u8 n <= 10, u16 n <= 1102, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

typedef struct {
    seq_t prod;   /* current product p * T_k           */
    seq_t Tk;     /* triangular number k(k+1)/2        */
    uint64_t prime; /* current prime p for this k       */
} node_t;

static inline void node_swap(node_t *a, node_t *b) {
    node_t t = *a;
    *a = *b;
    *b = t;
}

static void sift_down(node_t *h, size_t size, size_t i) {
    while (1) {
        size_t smallest = i;
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        if (left < size && h[left].prod < h[smallest].prod)
            smallest = left;
        if (right < size && h[right].prod < h[smallest].prod)
            smallest = right;
        if (smallest == i)
            break;
        node_swap(&h[i], &h[smallest]);
        i = smallest;
    }
}

static void sift_up(node_t *h, size_t i) {
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (h[i].prod >= h[parent].prod)
            break;
        node_swap(&h[i], &h[parent]);
        i = parent;
    }
}

seq_t a(idx_t n) {
    if (n == 0)                /* offset is 1; undefined for 0 */
        return 0;

    /* Heuristic: use three times n plus a small constant to guarantee enough
       triangular indices for the first n distinct products. */
    size_t kmax = (size_t)(n * 3ULL + 10ULL);
    if (kmax == 0)              /* protect against n = 0, though handled above */
        kmax = 1;

    node_t *heap = (node_t *)malloc(kmax * sizeof(node_t));
    if (!heap)
        return 0;               /* allocation failure – unlikely for small n */

    /* Initialize heap: for each k we start with prime = 2. */
    for (size_t k = 1; k <= kmax; ++k) {
        seq_t Tk = (seq_t)k * ((seq_t)k + 1) / 2; /* k(k+1)/2   */
        heap[k - 1].Tk   = Tk;
        heap[k - 1].prime = 2ULL;
        heap[k - 1].prod = (seq_t)2ULL * Tk;
    }
    /* Build a min‑heap. */
    for (size_t i = kmax / 2; i > 0; --i)
        sift_down(heap, kmax, i - 1);

    size_t heap_size = kmax;
    seq_t last = 0;      /* last distinct product we output */
    size_t count = 0;    /* how many distinct values produced */

    while (count < (size_t)n) {
        /* Extract the smallest product. */
        node_t cur = heap[0];
        heap[0] = heap[heap_size - 1];
        heap_size--;
        sift_down(heap, heap_size, 0);

        /* Count distinct products. */
        if (cur.prod != last) {
            last = cur.prod;
            count++;
            if (count == (size_t)n)
                break;
        }

        /* Advance to the next prime for this triangular index. */
        cur.prime = next_prime(cur.prime);
        cur.prod  = (seq_t)cur.prime * cur.Tk;

        /* Re‑insert. */
        heap[heap_size++] = cur;
        sift_up(heap, heap_size - 1);
    }

    free(heap);
    return last;
}
