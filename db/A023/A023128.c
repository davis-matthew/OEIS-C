/*
 * A023128  Signature sequence of 1/sqrt(6) (arrange the numbers i+j*x (i,j >=
 *          1) in increasing order; the sequence of i's is the signature of
 *          x).
 * https://oeis.org/A023128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   visited_test
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Signature sequence of 1/sqrt(6).
   Compute the n‑th term by generating all pairs (i,j) (i,j ≥ 1) in
   increasing order of i + j / sqrt(6).  The sequence a(n) is the
   i‑coordinate of the n‑th such pair.  The algorithm uses a binary
   min‑heap and a bitset to avoid duplicates.  Complexity: O(n log n). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/visited_test.h"

/* Node stores one pair (i,j). */
typedef struct { idx_t i, j; } Node;

/*--- heap utilities -----------------------------------------------------*/
static inline void heap_swap(Node *a, Node *b) {
    Node tmp = *a; *a = *b; *b = tmp;
}

/* Compare two nodes in order of i + j / sqrt(6).  When the sums are
   equal, the one with smaller i is considered smaller.  The comparison
   avoids floating point by using the identity
   (a + b/√6) < (c + d/√6) ⇔ (a-c) < (d-b)/√6 ⇔
   6*(a-c)^2 < (d-b)^2  when (a-c) > 0, etc. */
static int cmp_nodes(const Node *p1, const Node *p2) {
    int64_t da = (int64_t)p1->i - (int64_t)p2->i;
    int64_t db = (int64_t)p1->j - (int64_t)p2->j;
    if (da == 0) {
        return (db > 0) - (db < 0);                 /* j1 < j2 → -1, j1==j2→0, j1>j2→1 */
    }
    if (da > 0) {                                   /* a > b when L>0 */
        if (-db <= 0) return 1;                     /* a > b when R ≤ 0  */
        __int128 left  = (__int128)6 * (__int128)da * (__int128)da; /* 6*L² */
        __int128 right = (__int128)db * (__int128)db;              /* R² */
        if (left < right) return -1;
        if (left > right) return  1;
        return 0;                                   /* impossible for equal sums */
    } else {                                         /* da < 0 */
        if (-db >= 0) return -1;                    /* a < b when R ≥ 0  */
        __int128 left  = (__int128)6 * (__int128)da * (__int128)da;
        __int128 right = (__int128)db * (__int128)db;
        if (left > right) return -1;
        if (left < right) return  1;
        return 0;
    }
}

static inline void sift_up(Node *heap, idx_t idx) {
    while (idx > 0) {
        idx_t parent = (idx - 1) >> 1;
        if (cmp_nodes(&heap[idx], &heap[parent]) < 0) {
            heap_swap(&heap[idx], &heap[parent]);
            idx = parent;
        } else break;
    }
}

static inline void sift_down(Node *heap, idx_t idx, idx_t size) {
    while (1) {
        idx_t left = (idx << 1) + 1;
        if (left >= size) break;
        idx_t right = left + 1;
        idx_t smallest = left;
        if (right < size && cmp_nodes(&heap[right], &heap[left]) < 0)
            smallest = right;
        if (cmp_nodes(&heap[smallest], &heap[idx]) < 0) {
            heap_swap(&heap[smallest], &heap[idx]);
            idx = smallest;
        } else break;
    }
}

static inline void heap_push(Node *heap, idx_t *size, const Node *n) {
    heap[*size] = *n;
    sift_up(heap, (*size)++);
}

static inline Node heap_pop(Node *heap, idx_t *size) {
    Node top = heap[0];
    heap[0] = heap[--(*size)];
    sift_down(heap, 0, *size);
    return top;
}

static inline void visited_set(idx_t i, idx_t j, idx_t lim, uint64_t *vis) {
    size_t pos = (size_t)(i - 1) * (size_t)lim + (size_t)(j - 1);
    size_t idx = pos >> 6;
    uint64_t mask = 1ULL << (pos & 63);
    vis[idx] |= mask;
}

/*--- main sequence routine ------------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) return 0; /* the sequence is defined for n ≥ 1 */

    /* Bounds: i and j cannot exceed n, so a little extra margin is safe. */
    idx_t lim = n + 2;
    size_t words = ((size_t)lim * (size_t)lim + 63) >> 6;
    uint64_t *visited = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!visited) return 0;   /* allocation failure fallback */

    idx_t heap_cap = n + 10;
    Node *heap = (Node *)malloc(sizeof(Node) * heap_cap);
    if (!heap) {
        free(visited);
        return 0;   /* allocation failure fallback */
    }
    idx_t heap_size = 0;

    /* Start from the first pair (1,1). */
    Node init = {1, 1};
    heap_push(heap, &heap_size, &init);
    visited_set(1, 1, lim, visited);

    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        Node cur = heap_pop(heap, &heap_size);
        if (k == n) {
            result = (seq_t)cur.i;
            break;
        }
        /* Next candidates: (i+1, j) and (i, j+1) */
        idx_t ip = cur.i + 1;
        if (ip < lim && !visited_test(ip, cur.j, lim, visited)) {
            Node next = {ip, cur.j};
            heap_push(heap, &heap_size, &next);
            visited_set(ip, cur.j, lim, visited);
        }
        idx_t jp = cur.j + 1;
        if (jp < lim && !visited_test(cur.i, jp, lim, visited)) {
            Node next = {cur.i, jp};
            heap_push(heap, &heap_size, &next);
            visited_set(cur.i, jp, lim, visited);
        }
    }

    free(heap);
    free(visited);
    return result;
}
