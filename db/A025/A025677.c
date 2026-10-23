/*
 * A025677  Exponent of 8 (value of i) in n-th number of form 8^i*10^j.
 * https://oeis.org/A025677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025677:
   exponent of 8 in the n‑th smallest number of the form 8^i * 10^j.
   Computed by generating the numbers with a min‑heap; values are kept
   as logarithmic weights to avoid overflow. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*--- Visited‑pair hash table --------------------------------------------*/
#define VIS_TABLE_BITS 18
#define VIS_TABLE_SIZE (1U << VIS_TABLE_BITS)

static uint64_t _vis_keys[VIS_TABLE_SIZE];
static unsigned char _vis_used[VIS_TABLE_SIZE];

static int _visited(idx_t i, idx_t j)
{
    uint64_t key = (((uint64_t)i) << 32) | (uint64_t)j;
    size_t pos = key & (VIS_TABLE_SIZE - 1);
    while (_vis_used[pos]) {
        if (_vis_keys[pos] == key) return 1;
        pos = (pos + 1) & (VIS_TABLE_SIZE - 1);
    }
    _vis_used[pos] = 1;
    _vis_keys[pos] = key;
    return 0;
}

/*--- Main sequence function ------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* undefined, below offset */

    /* Logarithmic weight for 8 = log10(8).  10 shifts by +1.0. */
    const double LOG10_8 = 0.9030899872193768;

    /* Node in the heap: logarithmic value and exponents i,j. */
    struct Node {
        double weight;
        idx_t i;
        idx_t j;
    };

    size_t heap_cap = 3 * (size_t)n + 16;
    struct Node *heap = (struct Node *)malloc(heap_cap * sizeof(struct Node));
    if (!heap) return (seq_t)0;        /* allocation failure – unlikely */

    /* Initialise visited table. */
    memset(_vis_used, 0, sizeof(_vis_used));

    /* Heap initialisation. */
    size_t heap_sz = 0;
    heap[heap_sz++] = (struct Node){0.0, 0, 0};
    _visited(0, 0);

    seq_t result = 0;
    size_t cnt = 0;

    while (heap_sz && cnt < n) {
        /* Pop the minimum weight. */
        struct Node node = heap[0];
        heap[0] = heap[--heap_sz];
        size_t idx = 0;
        while (1) {
            size_t l = idx * 2 + 1;
            size_t r = l + 1;
            size_t s = idx;
            if (l < heap_sz && heap[l].weight < heap[s].weight) s = l;
            if (r < heap_sz && heap[r].weight < heap[s].weight) s = r;
            if (s == idx) break;
            struct Node t = heap[idx];
            heap[idx] = heap[s];
            heap[s] = t;
            idx = s;
        }

        /* Record answer for this n‑th term. */
        result = (seq_t)node.i;
        ++cnt;
        if (cnt == n) break;

        /* Generate neighbors (i+1, j) and (i, j+1). */
        idx_t i1 = node.i + 1, j1 = node.j;
        if (!_visited(i1, j1)) {
            if (heap_sz == heap_cap) {
                heap_cap *= 2;
                heap = (struct Node *)realloc(heap, heap_cap * sizeof(struct Node));
                if (!heap) break; /* unlikely, but keep safe */
            }
            double w = node.weight + LOG10_8;
            heap[heap_sz++] = (struct Node){w, i1, j1};
            size_t p = heap_sz - 1;
            while (p > 0) {
                size_t parent = (p - 1) >> 1;
                if (heap[parent].weight <= heap[p].weight) break;
                struct Node t = heap[parent];
                heap[parent] = heap[p];
                heap[p] = t;
                p = parent;
            }
        }

        i1 = node.i; j1 = node.j + 1;
        if (!_visited(i1, j1)) {
            if (heap_sz == heap_cap) {
                heap_cap *= 2;
                heap = (struct Node *)realloc(heap, heap_cap * sizeof(struct Node));
                if (!heap) break;
            }
            double w = node.weight + 1.0;
            heap[heap_sz++] = (struct Node){w, i1, j1};
            size_t p = heap_sz - 1;
            while (p > 0) {
                size_t parent = (p - 1) >> 1;
                if (heap[parent].weight <= heap[p].weight) break;
                struct Node t = heap[parent];
                heap[parent] = heap[p];
                heap[p] = t;
                p = parent;
            }
        }
    }

    free(heap);
    return result;
}
