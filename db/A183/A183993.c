/*
 * A183993  "Types" (parities of i and j) when all the numbers i+j*r are
 *          ranked, where r=golden ratio, i>=0, j>=0.
 * https://oeis.org/A183993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = type of parity of i and j for the n‑th smallest i + j*φ
 *            (φ = (1+sqrt(5))/2). 1 if i odd and j odd; 2 if i odd and j
 *            even; 3 if i even and j odd; 4 if i even and j even
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the first n entries by a min‑heap over all pairs
   (i,j) with j ∈ [0, n‑1] and i ≥ 0.  The heap key is the scaled value
   2*i*S + j*(1+sqrt5)*S (S = 10⁹), which preserves the ordering of i + jφ
   without floating point operations.  The algorithm is O(n log n).  */

#include <stdlib.h>
#include <stdbool.h>

/* Constants for scaling: φ ≈ (1+√5)/2.  Use S = 1e9 so all values fit
   into a 64‑bit unsigned integer for n up to millions. */
static const unsigned long long SCALE = 1000000000ULL;          /* 1e9 */
static const unsigned long long SQR5  = 2236067977ULL;          /* floor(√5 * 1e9) */
static const unsigned long long ONEPB = SCALE + SQR5;           /* (1+√5) * 1e9 */

/* Compute the scaled key for pair (i,j).  The comparison of these keys is
   equivalent to comparing the real values i + jφ. */
static inline unsigned long long scaled(idx_t i, idx_t j)
{
    return 2ULL * i * SCALE + j * ONEPB;
}

/* Heap node holding a pair (i,j) and its key. */
typedef struct {
    idx_t i, j;
    unsigned long long key;
} HeapNode;

/* Percolate up in the binary min‑heap. */
static inline void sift_up(HeapNode *h, size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (h[parent].key <= h[idx].key) break;
        HeapNode tmp = h[parent];
        h[parent] = h[idx];
        h[idx] = tmp;
        idx = parent;
    }
}

/* Percolate down in the binary min‑heap. */
static inline void sift_down(HeapNode *h, size_t idx, size_t size)
{
    while (1) {
        size_t left  = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;
        if (left < size && h[left].key < h[smallest].key) smallest = left;
        if (right < size && h[right].key < h[smallest].key) smallest = right;
        if (smallest == idx) break;
        HeapNode tmp = h[idx];
        h[idx] = h[smallest];
        h[smallest] = tmp;
        idx = smallest;
    }
}

/* Push a node onto the heap.  The caller guarantees enough capacity. */
static inline void heap_push(HeapNode *h, size_t *size, idx_t i, idx_t j)
{
    h[*size].i   = i;
    h[*size].j   = j;
    h[*size].key = scaled(i, j);
    sift_up(h, *size);
    (*size)++;
}

/* Pop the minimum node from the heap. */
static inline HeapNode heap_pop(HeapNode *h, size_t *size)
{
    HeapNode top = h[0];
    (*size)--;
    if (*size > 0) {
        h[0] = h[*size];
        sift_down(h, 0, *size);
    }
    return top;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1 */

    size_t target = (size_t)n;
    size_t heap_cap = target * 2;         /* sufficient for all pushes */
    HeapNode *heap = (HeapNode *)malloc(heap_cap * sizeof(HeapNode));
    size_t heap_size = 0;

    /* Initialise heap with all j from 0 to n-1, each starting at i = 0. */
    for (idx_t j = 0; j < target; ++j) {
        heap_push(heap, &heap_size, 0, j);
    }

    seq_t ans = 0;
    for (size_t cnt = 1; cnt <= target; ++cnt) {
        HeapNode cur = heap_pop(heap, &heap_size);
        if (cnt == target) {
            bool iodd = cur.i & 1;
            bool jok  = cur.j & 1;
            if (iodd) {
                ans = jok ? 1 : 2;
            } else {
                ans = jok ? 3 : 4;
            }
            break;
        }
        /* Push the next pair in this column (i+1, j). */
        heap_push(heap, &heap_size, cur.i + 1, cur.j);
    }

    free(heap);
    return ans;
}
