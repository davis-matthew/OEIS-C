/*
 * A090466  Regular figurative or polygonal numbers of order greater than 2.
 * https://oeis.org/A090466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 151, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A090466 (Regular figurative/​polygonal numbers of order > 2).
   Uses a min‑heap over the k‑gonal number sequences (rank ≥ 3). */

#include <stdlib.h>

struct Node { idx_t val; idx_t k; idx_t n; };

static inline void node_swap(struct Node *a, struct Node *b) {
    struct Node tmp = *a; *a = *b; *b = tmp;
}

static inline int node_cmp(const struct Node *a, const struct Node *b) {
    return (a->val < b->val) ? -1 : (a->val > b->val) ? 1 : 0;
}

static void heap_push(struct Node *h, size_t *size, struct Node item) {
    size_t i = (*size)++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (node_cmp(&item, &h[p]) < 0) {
            h[i] = h[p];
            i = p;
        } else break;
    }
    h[i] = item;
}

static struct Node heap_pop(struct Node *h, size_t *size) {
    struct Node res = h[0];
    size_t n = --(*size);
    if (n > 0) {
        h[0] = h[n];
        size_t i = 0;
        while (i < n / 2) {
            size_t lc = 2 * i + 1;
            size_t rc = lc + 1;
            size_t sm = lc;
            if (rc < n && node_cmp(&h[rc], &h[lc]) < 0) sm = rc;
            if (node_cmp(&h[sm], &h[i]) >= 0) break;
            node_swap(&h[i], &h[sm]);
            i = sm;
        }
    }
    return res;
}

static inline struct Node next_node(const struct Node cur) {
    idx_t new_n = cur.n + 1;
    idx_t delta = (cur.k - 2) * cur.n + 1;
    idx_t new_val = cur.val + delta;
    struct Node nxt = { new_val, cur.k, new_n };
    return nxt;
}

/* Return the n-th term of the sequence (1‑based indexing). */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* A reasonable upper bound for the number of k‑gonals needed. */
    size_t KMAX = (size_t)(n * 2 + 10);
    struct Node *heap = (struct Node *)malloc((KMAX + 1) * sizeof(struct Node));
    if (!heap) return 0;
    size_t heap_sz = 0;

    /* Initial term for each k (rank = 3). */
    for (idx_t k = 3; k <= KMAX; ++k) {
        idx_t first = 3 * k - 3;
        heap_push(heap, &heap_sz, (struct Node){ first, k, 3 });
    }

    idx_t last = 0;
    int have_last = 0;
    idx_t count = 0;

    while (count < n && heap_sz > 0) {
        struct Node cur = heap_pop(heap, &heap_sz);
        if (have_last && cur.val == last) {
            struct Node nxt = next_node(cur);
            heap_push(heap, &heap_sz, nxt);
            continue;  /* duplicate value – not counted */
        }
        /* Accept this distinct value. */
        count++;
        last = cur.val;
        have_last = 1;
        struct Node nxt = next_node(cur);
        heap_push(heap, &heap_sz, nxt);
    }

    free(heap);
    return (seq_t)last;
}
