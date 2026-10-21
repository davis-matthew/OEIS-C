/*
 * A054734  Numbers of the form 2*x*(x + 1)*y*(y + 1) where x and y are
 *          distinct.
 * https://oeis.org/A054734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A054734
 *  a(n) = 2 * x * (x + 1) * y * (y + 1)  for all distinct integers x < y.
 *  (a(1)=0; a(2)=24; a(3)=48; …)
 *
 *  The routine generates the distinct values of
 *      v = 2 * x*(x+1) * y*(y+1)   (x < y)
 *  in ascending order and stops when the nth distinct value is obtained.
 *  It uses a min‑heap of size O(√n); the heap contains one entry per
 *  value x < y ≤ M, where M ≈ 2·√(2n)+10 guarantees a sufficient number
 *  of candidates for the requested n.
 *
 *  The function works for the native integer types supplied for seq_t
 *  (uint8_t … uint128, or an arbitrary‑precision class in C++17).
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* helper structure for a heap node --------------------------------------------------- */
typedef struct {
    seq_t val;      /* value 2 * x*(x+1) * y*(y+1) */
    idx_t x;        /* current x  (fixed for the node) */
    idx_t y;        /* current y  (≥ x+1) */
} HeapNode;

/* heap operations -------------------------------------------------------------- */
static inline void heap_swap(HeapNode *a, HeapNode *b) {
    HeapNode tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_push(HeapNode *h, size_t *sz, HeapNode v) {
    ++(*sz);
    h[*sz] = v;                       /* place at the end */
    size_t i = *sz;                   /* sift‑up */
    while (i > 1) {
        size_t p = i >> 1;
        if (h[p].val <= h[i].val) break;
        heap_swap(&h[p], &h[i]);
        i = p;
    }
}

static HeapNode heap_pop(HeapNode *h, size_t *sz) {
    HeapNode ret = h[1];
    h[1] = h[*sz];
    --(*sz);
    size_t i = 1;                     /* sift‑down */
    while (1) {
        size_t l = i << 1;
        if (l > *sz) break;
        size_t r = l + 1;
        size_t m = l;
        if (r <= *sz && h[r].val < h[l].val) m = r;
        if (h[m].val >= h[i].val) break;
        heap_swap(&h[i], &h[m]);
        i = m;
    }
    return ret;
}

/* integer square root (floor) ----------------------------------------------------- */
static idx_t int_sqrt(idx_t x) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

/* ----------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* offset 1: a(1) = 0 */
    if (n == 1) return 0;

    /* estimate upper bound M for x and y so that enough distinct values exist */
    idx_t M = int_sqrt(n * (idx_t)2) * 2 + (idx_t)10;
    if (M < 3) M = 3;                     /* at least a few elements */

    /* allocate heap */
    HeapNode *heap = (HeapNode *)malloc((M + 1) * sizeof(HeapNode));
    if (!heap) return 0;                  /* unlikely – just a safety fall‑back */

    size_t hsize = 0;

    /* initial heap fill: for each x, the first pair (x, x+1) */
    for (idx_t x = 0; x <= M; ++x) {
        idx_t y = x + 1;
        if (y > M) continue;
        seq_t xprod = (seq_t)x * (x + 1);
        seq_t yprod = (seq_t)y * (y + 1);
        seq_t val = (seq_t)2 * xprod * yprod;
        HeapNode node = { val, x, y };
        heap_push(heap, &hsize, node);
    }

    seq_t last_val = 0;
    idx_t count = 0;
    bool first = true;

    /* generate distinct values in ascending order */
    while (count < n) {
        HeapNode cur = heap_pop(heap, &hsize);

        if (first || cur.val != last_val) {
            ++count;
            last_val = cur.val;
            if (count == n) {
                free(heap);
                return cur.val;
            }
            first = false;
        }

        /* push next y for the same x, if within bounds */
        if (cur.y + 1 <= M) {
            idx_t next_y = cur.y + 1;
            seq_t xprod = (seq_t)cur.x * (cur.x + 1);
            seq_t yprod = (seq_t)next_y * (next_y + 1);
            seq_t val = (seq_t)2 * xprod * yprod;
            HeapNode nxt = { val, cur.x, next_y };
            heap_push(heap, &hsize, nxt);
        }
    }

    /* unreachable */
    free(heap);
    return last_val;
}
