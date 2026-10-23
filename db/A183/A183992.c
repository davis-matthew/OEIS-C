/*
 * A183992  Ranks of (even i)+(even j)*r when all i+j*r are ranked, where
 *          r=golden ratio, i>=0, j>=0.
 * https://oeis.org/A183992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the rank of the n‑th pair (i,j) with both i and j even
   in the ordering of all pairs (i,j) by the value i + j*φ, where φ is the
   golden ratio.  The function performs a heap‑based generation of the
   sorted sequence and stops when the n‑th even‑even pair is found. */

#include <stdlib.h>
#include <math.h>

/* Heap node: current i, j, and the floating value i + j*φ */
typedef struct {
    idx_t    i;
    idx_t    j;
    double   val;
} heap_node_t;

/* Restore heap property from node pos downwards.  idx_t may be 64bit. */
static void sift_down(heap_node_t *h, idx_t size, idx_t pos)
{
    while (1) {
        idx_t left = pos * 2 + 1;
        if (left >= size) break;
        idx_t right = left + 1;
        idx_t smallest = left;
        if (right < size && h[right].val < h[left].val)
            smallest = right;
        if (h[pos].val <= h[smallest].val) break;
        heap_node_t tmp = h[pos];
        h[pos] = h[smallest];
        h[smallest] = tmp;
        pos = smallest;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<1, but harmless */

    /* Golden ratio */
    static const double phi = (1.0 + sqrt(5.0)) / 2.0;

    /* Upper bound on j needed for the n-th even‑even pair.  The density
       of even–even pairs is about 1/4 of all pairs, so 4*n is a safe buffer. */
    idx_t jmax = n * 4 + 10;
    if (jmax == 0) jmax = 1;

    /* Allocate heap with one entry per j (0 … jmax). */
    heap_node_t *heap = (heap_node_t *)malloc((size_t)(jmax + 1) * sizeof(heap_node_t));
    if (!heap) return 0;  /* unlikely, but keeps the compiler happy */

    /* Initialise heap: for each j start with i = 0. */
    for (idx_t j = 0; j <= jmax; ++j) {
        heap[j].i   = 0;
        heap[j].j   = j;
        heap[j].val = j * phi;
    }
    idx_t heap_size = jmax + 1;

    /* Build min‑heap. */
    for (idx_t k = heap_size / 2; k > 0; --k)
        sift_down(heap, heap_size, k - 1);

    idx_t rank      = 0;  /* 1‑based rank in the ordering */
    idx_t even_cnt  = 0;  /* number of even‑even pairs seen */

    while (1) {
        /* Extract minimum element (current pair). */
        heap_node_t cur = heap[0];
        ++rank;

        /* Check if both coordinates are even. */
        if (((cur.i & 1) == 0) && ((cur.j & 1) == 0)) {
            ++even_cnt;
            if (even_cnt == n) {
                free(heap);
                return (seq_t)rank;
            }
        }

        /* Advance this node to the next i in its j‑sequence. */
        ++cur.i;
        cur.val += 1.0;   /* i increases by 1 */

        heap[0] = cur;
        sift_down(heap, heap_size, 0);
    }

    /* Unreachable, but keep compiler happy. */
    free(heap);
    return 0;
}
