/*
 * A084579  Let y = m/GK(k), where m and k vary over the positive integers and
 *          GK(k)=log(1+1/(k*(k+2)))/log(2) is the Gauss-Kuzmin distribution
 *          of k. Sort the y values by size and number them consecutively by
 *          n. This sequence gives the values of m in order by n.
 * https://oeis.org/A084579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth value of m when all values m / GK(k) (k,m∈ℕ⁺)
   are sorted increasingly, where GK(k)=log₂(1+1/(k(k+2))).  The
   function returns that m for the given index n. */
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* A helper structure for a min‑heap node */
typedef struct {
    double y;      /* the value m / GK(k) */
    idx_t   k;     /* the k index (1‑based) */
    idx_t   m;     /* the current m for this k */
} node_t;

/* Min‑heap insertion: heap size 'sz' grows by one */
static void heap_push(node_t *heap, size_t *sz, node_t ele)
{
    heap[(*sz)++] = ele;
    size_t i = *sz - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].y <= heap[i].y)
            break;
        node_t tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

/* Min‑heap extraction: returns the smallest element */
static node_t heap_pop(node_t *heap, size_t *sz)
{
    node_t res = heap[0];
    heap[0] = heap[--(*sz)];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < *sz && heap[l].y < heap[smallest].y)
            smallest = l;
        if (r < *sz && heap[r].y < heap[smallest].y)
            smallest = r;
        if (smallest == i)
            break;
        node_t tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Any n < 1 is outside the official offset; return 0. */
    if (n < 1) return (seq_t)0;

    /* Upper bound on k: 2*n is more than enough for n ≤ 10⁴ */
    idx_t K_MAX = n * 2;
    if (K_MAX < 2) K_MAX = 2;

    /* Pre‑compute GK(k) for k = 1 … K_MAX */
    double *gk = (double *)malloc((K_MAX + 1) * sizeof(double));
    if (!gk) return (seq_t)0;                /* allocation failed */
    static const double LOG2 = 0.69314718055994530942; /* log(2) */
    for (idx_t k = 1; k <= K_MAX; ++k) {
        double val = 1.0 / (k * (k + 2));
        gk[k] = log1p(val) / LOG2;           /* GK(k) */
    }

    /* Build the initial heap: one entry for each k with m = 1 */
    size_t heap_sz = 0;
    node_t *heap = (node_t *)malloc((K_MAX + 1) * sizeof(node_t));
    if (!heap) { free(gk); return (seq_t)0; }

    for (idx_t k = 1; k <= K_MAX; ++k) {
        node_t node;
        node.k = k;
        node.m = 1;
        node.y = 1.0 / gk[k];
        heap_push(heap, &heap_sz, node);
    }

    seq_t result = 0;   /* holds a(n) when finished */

    for (idx_t step = 0; step < n; ++step) {
        node_t cur = heap_pop(heap, &heap_sz);
        if (step == n - 1)
            result = (seq_t)cur.m;   /* the requested term */

        /* Push the next element of the same k */
        node_t next;
        next.k = cur.k;
        next.m = cur.m + 1;
        next.y = (double)(cur.m + 1) / gk[cur.k];
        heap_push(heap, &heap_sz, next);
    }

    free(heap);
    free(gk);
    return result;
}
