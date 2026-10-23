/*
 * A025675  Exponent of 8 (value of j) in n-th number of form 7^i*8^j.
 * https://oeis.org/A025675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   visited_insert
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025675:  exponent of 8 in the n‑th smallest number of the form 7^i·8^j.
   This implementation generates the pairs (i,j) in increasing order of 7^i·8^j
   using a min‑heap keyed by i·ln7 + j·ln8.  The j coordinate of the nth
   popped pair is the desired value.  The algorithm runs in O(n log n) time
   and uses only integer types. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/visited_insert.h"

/* Node stored in the heap.  The weight is a double approximation of the
   logarithm of 7^i * 8^j.  The double is used only for ordering; the
   exact value is not needed. */
typedef struct {
    idx_t i;          /* exponent of 7 */
    idx_t j;          /* exponent of 8 */
    double w;         /* i·ln7 + j·ln8 */
} Node;

/* ---------- heap utilities ---------- */
static void heap_push(Node *h, size_t *sz, Node v)
{
    h[(*sz)++] = v;
    size_t idx = *sz - 1;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (h[parent].w <= h[idx].w) break;
        Node tmp = h[parent]; h[parent] = h[idx]; h[idx] = tmp;
        idx = parent;
    }
}

static Node heap_pop(Node *h, size_t *sz)
{
    Node ret = h[0];
    h[0] = h[--(*sz)];
    size_t idx = 0;
    while (1) {
        size_t left = (idx << 1) + 1;
        if (left >= *sz) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < *sz && h[right].w < h[left].w) smallest = right;
        if (h[smallest].w >= h[idx].w) break;
        Node tmp = h[smallest]; h[smallest] = h[idx]; h[idx] = tmp;
        idx = smallest;
    }
    return ret;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n<1, guard */

    /* logarithms of 7 and 8, computed once. */
    const double LOG7 = 1.945910149055313305; /* log(7)   */
    const double LOG8 = 2.079441541679835928; /* log(8)   */

    /* Heap capacity: at most twice the number of required elements. */
    size_t heap_cap = n * 2 + 16;
    Node *heap = (Node *)malloc(heap_cap * sizeof(Node));
    size_t heap_sz = 0;

    /* Visited table size: power of two, at least 8×n. */
    size_t v_cap = 1;
    while (v_cap < n * 8) v_cap <<= 1;
    uint64_t *visited = (uint64_t *)calloc(v_cap, sizeof(uint64_t));

    /* start with (0,0) */
    Node start = {0, 0, 0.0};
    visited_insert(0, visited, v_cap);
    heap_push(heap, &heap_sz, start);

    seq_t result = 0;
    idx_t cnt = 0;

    while (heap_sz) {
        Node cur = heap_pop(heap, &heap_sz);
        ++cnt;
        if (cnt == n) {
            result = (seq_t)cur.j;
            break;
        }

        /* child (i+1, j) */
        idx_t ni = cur.i + 1;
        uint64_t key1 = ((uint64_t)ni << 32) | cur.j;
        if (visited_insert(key1, visited, v_cap))
            heap_push(heap, &heap_sz,
                      (Node){ni, cur.j, cur.w + LOG7});

        /* child (i, j+1) */
        idx_t nj = cur.j + 1;
        uint64_t key2 = ((uint64_t)cur.i << 32) | nj;
        if (visited_insert(key2, visited, v_cap))
            heap_push(heap, &heap_sz,
                      (Node){cur.i, nj, cur.w + LOG8});
    }

    free(heap);
    free(visited);
    return result;
}
