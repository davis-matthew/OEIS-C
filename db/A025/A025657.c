/*
 * A025657  Exponent of 6 (value of j) in n-th number of form 3^i*6^j.
 * https://oeis.org/A025657
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

/* Computes a(n) for sequence A025657 (exponent of 6 in the nth smallest
   number of the form 3^i * 6^j) by generating the sorted list on the fly with
   a binary heap.  The algorithm uses only 64‑bit arithmetic for indices
   and stores the logarithm of the value for ordering, so it works with
   both 64‑bit and arbitrary‑precision builds. */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>

/* Limits for the visited array – large enough for the expected range
   of i and j (n up to 10 000).  The array uses 4 MB global storage. */
#define MAX_I 1024
#define MAX_J 1024

/* Global visited table and a token counter to avoid clearing it for every
   call.  visited[i][j] contains the token of the last call that marked
   the pair (i,j). */
static uint32_t visited[MAX_J][MAX_I] = {0};
static uint32_t next_token = 1;

typedef struct {
    double logval;   /* logarithm of 3^i * 6^j for ordering */
    idx_t i;
    idx_t j;
} Node;

/* swap two nodes */
static inline void swap_node(Node *a, Node *b)
{
    Node tmp = *a;
    *a = *b;
    *b = tmp;
}

/* push a node into the binary min‑heap */
static void heap_push(Node *heap, size_t *size, const Node *node)
{
    size_t idx = *size;
    heap[idx] = *node;
    (*size)++;

    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].logval <= heap[idx].logval)
            break;
        swap_node(&heap[parent], &heap[idx]);
        idx = parent;
    }
}

/* pop the smallest node from the heap */
static Node heap_pop(Node *heap, size_t *size)
{
    Node top = heap[0];
    (*size)--;
    heap[0] = heap[*size];
    size_t idx = 0;
    while (1) {
        size_t left = (idx << 1) + 1;
        if (left >= *size)
            break;
        size_t right = left + 1;
        size_t child = left;
        if (right < *size && heap[right].logval < heap[left].logval)
            child = right;
        if (heap[idx].logval <= heap[child].logval)
            break;
        swap_node(&heap[idx], &heap[child]);
        idx = child;
    }
    return top;
}

seq_t a(idx_t n)
{
    /* Safety check: n must be at least 1 */
    if (n == 0)
        return (seq_t)0;

    const double LOG3 = log(3.0);
    const double LOG6 = log(6.0);

    /* Allocate a heap large enough to hold at most 2n nodes */
    size_t heap_capacity = (size_t)n * 2 + 10;
    Node *heap = (Node *)malloc(heap_capacity * sizeof(Node));
    if (!heap)
        return (seq_t)0;           /* allocation failure falls back to 0 */

    /* Get a fresh token for this call and mark the start pair */
    uint32_t my_token = next_token++;
    visited[0][0] = my_token;

    size_t heap_size = 0;
    Node root = {0.0, 0, 0};
    heap_push(heap, &heap_size, &root);

    /* Generate numbers until we reach the nth one */
    for (idx_t count = 1; count <= n; ++count) {
        Node cur = heap_pop(heap, &heap_size);

        if (count == n) {
            free(heap);
            return (seq_t)cur.j;     /* j is the answer */
        }

        /* child (i+1, j) – multiply by 3 */
        if (cur.i + 1 < MAX_I) {
            if (visited[cur.i + 1][cur.j] != my_token) {
                visited[cur.i + 1][cur.j] = my_token;
                Node child = {cur.logval + LOG3, cur.i + 1, cur.j};
                heap_push(heap, &heap_size, &child);
            }
        }

        /* child (i, j+1) – multiply by 6 */
        if (cur.j + 1 < MAX_J) {
            if (visited[cur.i][cur.j + 1] != my_token) {
                visited[cur.i][cur.j + 1] = my_token;
                Node child = {cur.logval + LOG6, cur.i, cur.j + 1};
                heap_push(heap, &heap_size, &child);
            }
        }
    }

    /* Should never reach here */
    free(heap);
    return (seq_t)0;
}
