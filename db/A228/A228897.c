/*
 * A228897  Nodes of tree generated as follows: (1,2) is an edge, and if (x,y)
 *          is an edge, then (y,x+y) and (y,x*y) are edges.
 * https://oeis.org/A228897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th distinct integer appearing as either endpoint of
 *            an edge in the infinite binary tree defined by (1,2) and the
 *            rule (x,y) → (y,x+y) and (y,x*y). The sequence is sorted
 *            ascending
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ===== common definitions ===== */
typedef struct {
    seq_t x;
    seq_t y;
} edge_t;

#ifdef __cplusplus
/* ========= C++ implementation (uses std::priority_queue) ========= */
#include <queue>
#include <vector>
#include <cstddef>
#include <limits>

struct edge_cmp {
    bool operator()(const edge_t& a, const edge_t& b) const {
        return a.y > b.y;                 /* min‑heap on the y‑value */
    }
};

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;           /* a(1) = 1 */

    std::priority_queue<edge_t, std::vector<edge_t>, edge_cmp> pq;
    pq.push({(seq_t)1, (seq_t)2});          /* root edge (1,2) */

    idx_t count = 1;                        /* node 1 already counted */
    seq_t last_y = (seq_t)0;                /* sentinel */

    while (true) {
        edge_t e = pq.top(); pq.pop();
        seq_t x = e.x, y = e.y;

        if (y != last_y) {
            ++count;
            last_y = y;
            if (count == n) return y;
        }

        /* child (y, x + y) – guard against 64‑bit overflow */
        bool ok_add = true;
        seq_t sum = 0;
        if (sizeof(seq_t) <= sizeof(std::uint64_t)) {
            if (x > (seq_t)std::numeric_limits<std::uint64_t>::max() - y)
                ok_add = false;
            else
                sum = x + y;
        } else {
            sum = x + y;
        }
        if (ok_add) pq.push({y, sum});

        /* child (y, x * y) – guard against 64‑bit overflow */
        bool ok_mul = true;
        seq_t prod = 0;
        if (sizeof(seq_t) <= sizeof(std::uint64_t)) {
            if (x == 0 || y == 0)
                prod = 0;
            else if ((seq_t)std::numeric_limits<std::uint64_t>::max() / y >= x)
                prod = x * y;
            else
                ok_mul = false;
        } else {
            prod = x * y;
        }
        if (ok_mul) pq.push({y, prod});
    }
}
#else
/* ========= C implementation (manual binary heap) ========= */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static void heap_push(edge_t **h, size_t *cap, size_t *sz, seq_t x, seq_t y)
{
    if (*sz == *cap) {
        size_t new_cap = (*cap == 0) ? 4 : *cap << 1;
        edge_t *tmp = (edge_t *)realloc(*h, new_cap * sizeof(edge_t));
        if (!tmp) exit(1);                 /* Out of memory */
        *h = tmp;
        *cap = new_cap;
    }
    size_t i = *sz;
    (*h)[i].x = x;
    (*h)[i].y = y;
    ++*sz;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if ((*h)[p].y <= (*h)[i].y) break;
        edge_t t = (*h)[p];
        (*h)[p] = (*h)[i];
        (*h)[i] = t;
        i = p;
    }
}

static edge_t heap_pop(edge_t *h, size_t *sz)
{
    edge_t r = h[0];
    --*sz;
    if (*sz == 0) return r;
    h[0] = h[*sz];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= *sz) break;
        size_t rgt = l + 1;
        size_t s = l;
        if (rgt < *sz && h[rgt].y < h[l].y) s = rgt;
        if (h[i].y <= h[s].y) break;
        edge_t t = h[i];
        h[i] = h[s];
        h[s] = t;
        i = s;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;           /* a(1) = 1 */

    size_t h_cap = 0, h_sz = 0;
    edge_t *heap = NULL;

    heap_push(&heap, &h_cap, &h_sz, (seq_t)1, (seq_t)2);   /* root edge */

    idx_t count = 1;                        /* node 1 already counted */
    seq_t last_y = (seq_t)0;                /* sentinel */

    while (1) {
        edge_t e = heap_pop(heap, &h_sz);
        seq_t x = e.x, y = e.y;

        if (y != last_y) {
            ++count;
            last_y = y;
            if (count == n) {
                free(heap);
                return y;
            }
        }

        /* child (y, x + y) – guard against 64‑bit overflow */
        bool ok_add = true;
        seq_t sum = 0;
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            if (x > (seq_t)UINT64_MAX - y) ok_add = false;
            else sum = x + y;
        } else
            sum = x + y;
        if (ok_add) heap_push(&heap, &h_cap, &h_sz, y, sum);

        /* child (y, x * y) – guard against 64‑bit overflow */
        bool ok_mul = true;
        seq_t prod = 0;
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            if (x == 0 || y == 0) prod = 0;
            else if ((seq_t)UINT64_MAX / y >= x) prod = x * y;
            else ok_mul = false;
        } else
            prod = x * y;
        if (ok_mul) heap_push(&heap, &h_cap, &h_sz, y, prod);
    }
}
#endif
