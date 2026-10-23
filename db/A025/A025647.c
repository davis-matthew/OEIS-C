/*
 * A025647  Exponent of 4 (value of i) in n-th number of form 4^i*6^j.
 * https://oeis.org/A025647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of 4 in the n-th smallest integer of the form
 *            4^i * 6^j (i,j ≥ 0). The sequence starts
 *            0,1,0,2,1,0,3,2,1,0,4,3,2,1,5,0,4,3,2,6,1,5,0,4,
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* calloc, free, NULL */
#include <stdbool.h>  /* bool, false, true */
#include <math.h>     /* logl, fabsl */
#include <stddef.h>   /* size_t */

/* Node used in the heap: only the exponents are stored,
   the actual value is compared via logarithms to avoid overflow. */
typedef struct {
    idx_t i;   /* exponent of 4 (2^i * 2^i = 4^i) */
    idx_t j;   /* exponent of 6 (3^j * 2^j = 6^j) */
} Node;

/* Pre‑computed logarithms used for comparison. */
static const long double LOG_4 = logl(4.0L);
static const long double LOG_6 = logl(6.0L);

/* Tolerance for floating‑point comparison.  Small enough that
   it is far below the spacing of long double for the ranges of
   interest (i,j ≤ 1e5). */
static const long double EPS = 1e-12L;

/* Compare two nodes according to their real value 4^i * 6^j.
   Uses long double logarithms to avoid overflow and
   provides a deterministic tie‑break. */
static inline int node_cmp(const Node *a, const Node *b)
{
    long double la = a->i * LOG_4 + a->j * LOG_6;
    long double lb = b->i * LOG_4 + b->j * LOG_6;
    long double diff = la - lb;

    if (fabsl(diff) > EPS)
        return diff < 0.0L ? -1 : 1;

    /* Numerical tie; use deterministic ordering by i then j. */
    if (a->i < b->i) return -1;
    if (a->i > b->i) return 1;
    if (a->j < b->j) return -1;
    if (a->j > b->j) return 1;
    return 0;
}

/* Heap helper functions ----------------------------------------------------*/
static void heap_push(Node *heap, size_t *size, size_t capacity, Node val)
{
    ++(*size);
    size_t pos = *size;
    heap[pos] = val;
    while (pos > 1) {
        size_t parent = pos >> 1;
        if (node_cmp(&heap[pos], &heap[parent]) < 0) {
            Node tmp = heap[pos];
            heap[pos] = heap[parent];
            heap[parent] = tmp;
            pos = parent;
        } else
            break;
    }
}

static Node heap_pop(Node *heap, size_t *size)
{
    Node ret = heap[1];
    heap[1] = heap[*size];
    --(*size);
    size_t pos = 1;
    while (1) {
        size_t left = pos << 1;
        if (left > *size)
            break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right <= *size && node_cmp(&heap[right], &heap[left]) < 0)
            smallest = right;
        if (node_cmp(&heap[smallest], &heap[pos]) < 0) {
            Node tmp = heap[smallest];
            heap[smallest] = heap[pos];
            heap[pos] = tmp;
            pos = smallest;
        } else
            break;
    }
    return ret;
}

/* Simple hash table for visited (i,j) pairs --------------------------------*/
static inline uint64_t encode_key(idx_t i, idx_t j)
{
    return (((uint64_t)i) << 32) | (uint64_t)j;
}

static int visited_insert(uint64_t key, uint64_t *table, size_t capacity)
{
    size_t idx = key & (capacity - 1);
    while (table[idx]) {
        if (table[idx] == key)
            return 0;          /* already visited */
        idx = (idx + 1) & (capacity - 1);
    }
    table[idx] = key;
    return 1;                  /* new insertion */
}

/* Main sequence function ---------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* offset starts at 1 */

    /* Estimate how many nodes we may need.  At worst we push ~2n nodes
       (each pop generates two successors).  Allocate a power of two
       larger than that for the visited hash table. */
    size_t vcap = 1;
    while (vcap < (size_t)n * 4)
        vcap <<= 1;
    uint64_t *visited = (uint64_t *)calloc(vcap, sizeof(uint64_t));
    if (!visited)
        return 0;  /* allocation failure */

    /* Heap capacity: one entry per node plus a safety margin. */
    size_t hcap = (size_t)n * 2 + 10;
    Node *heap = (Node *)calloc(hcap, sizeof(Node));
    if (!heap) {
        free(visited);
        return 0;
    }
    size_t heap_size = 0;

    /* Start from (0,0) which corresponds to 4^0*6^0 = 1. */
    {
        if (visited_insert(encode_key(0, 0), visited, vcap)) {
            Node start = {0, 0};
            heap_push(heap, &heap_size, hcap, start);
        }
    }

    seq_t answer = 0;

    for (idx_t cnt = 0; cnt < n; ++cnt) {
        Node cur = heap_pop(heap, &heap_size);
        answer = (seq_t)cur.i;          /* exponent of 4 at this rank */

        if (cnt + 1 == n)
            break;

        /* Generate successors (i+1, j) and (i, j+1). */
        {
            uint64_t key = encode_key(cur.i + 1, cur.j);
            if (visited_insert(key, visited, vcap)) {
                Node nxt = {cur.i + 1, cur.j};
                heap_push(heap, &heap_size, hcap, nxt);
            }
        }
        {
            uint64_t key = encode_key(cur.i, cur.j + 1);
            if (visited_insert(key, visited, vcap)) {
                Node nxt = {cur.i, cur.j + 1};
                heap_push(heap, &heap_size, hcap, nxt);
            }
        }
    }

    free(heap);
    free(visited);
    return answer;
}
