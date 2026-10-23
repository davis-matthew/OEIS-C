/*
 * A025690  Exponent of 10 (value of j) in n-th number of form 8^i*10^j.
 * https://oeis.org/A025690
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

/* Sequence A025690: exponent of 10 (j) in the n‑th number of the form 8^i·10^j.
   The algorithm merges the infinite set { 2^k·5^j : k≥j≥0 } in increasing order
   using a min‑heap keyed by the logarithmic value k·log 2 + j·log 5.  Each
   element (i,j) is visited once; its children (i+1,j) and (i,j+1) are pushed
   into the heap afterwards.  The function returns the j‑exponent of the n‑th
   smallest element. */

#include <stdlib.h>
#include <string.h>

/* Limits on the indices i and j used for memoisation.  These are large
   enough to cover all indices that occur for n up to at least 10 000. */
#define MAXI 2048
#define MAXJ 2048

/* Node used in the heap: (i,j) and its logarithmic key. */
typedef struct {
    idx_t i;
    idx_t j;
    long double key;
} Node;

/* Pre‑computed logarithms.  A long double is used to minimise precision
   loss when comparing keys of different (i,j). */
static const long double LOG2 = 0.6931471805599453094172321214581765680755001343602552L;
static const long double LOG5 = 1.609437912434100374600759333226187639525322438338L;

/* Global visited table to avoid inserting the same (i,j) twice.
   The table is cleared on every call to a(n). */
static unsigned char visited[MAXI][MAXJ];

/* Heap operations ---------------------------------------------------------- */
static inline void heap_push(Node *heap, size_t *size, Node v)
{
    size_t i = (*size)++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].key <= v.key) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i] = v;
}

static inline Node heap_pop(Node *heap, size_t *size)
{
    Node result = heap[0];
    Node last = heap[--(*size)];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        if (l >= *size) break;
        size_t child = r < *size && heap[r].key < heap[l].key ? r : l;
        if (last.key <= heap[child].key) break;
        heap[i] = heap[child];
        i = child;
    }
    heap[i] = last;
    return result;
}

/* Helper to create a node with its key. */
static inline Node make_node(idx_t i, idx_t j)
{
    Node n;
    n.i = i;
    n.j = j;
    n.key = ((long double)(3 * i + j)) * LOG2 + (long double)j * LOG5;
    return n;
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;  /* a(1) = 0 */

    /* Clear the visited table for this call. */
    memset(visited, 0, sizeof(visited));

    /* Allocate a heap capable of holding up to (n+1) nodes. */
    size_t heap_cap = (size_t)n + 1;
    Node *heap = (Node *)malloc(heap_cap * sizeof(Node));
    if (!heap) return (seq_t)0;  /* allocation failure, return 0 as safe fallback */

    /* Start with the smallest element (i=0,j=0). */
    visited[0][0] = 1;
    size_t heap_sz = 0;
    heap_push(heap, &heap_sz, make_node(0, 0));

    size_t count = 0;
    Node cur;

    while (1) {
        cur = heap_pop(heap, &heap_sz);
        ++count;
        if (count == n) break;

        /* Add children if within bounds and not yet visited. */
        if (cur.i + 1 < (idx_t)MAXI && !visited[(size_t)(cur.i + 1)][(size_t)cur.j]) {
            visited[(size_t)(cur.i + 1)][(size_t)cur.j] = 1;
            heap_push(heap, &heap_sz, make_node(cur.i + 1, cur.j));
        }
        if (cur.j + 1 < (idx_t)MAXJ && !visited[(size_t)cur.i][(size_t)(cur.j + 1)]) {
            visited[(size_t)cur.i][(size_t)(cur.j + 1)] = 1;
            heap_push(heap, &heap_sz, make_node(cur.i, cur.j + 1));
        }
    }

    free(heap);
    return (seq_t)cur.j;
}
