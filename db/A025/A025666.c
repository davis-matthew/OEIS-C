/*
 * A025666  Exponent of 7 (value of j) in n-th number of form 4^i*7^j.
 * https://oeis.org/A025666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = j where (i,j) is the n-th smallest product of 4^i*7^j
 *            Implementation: generate the sorted list of all numbers of the
 *            form 4^i*7^j using a min‑heap over the logarithmic value d =
 *            i*log(4) + j*log(7). Each (i,j) is inserted only once to avoid
 *            duplicates; a visited bitmap is used for that. Th...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* maximum i or j that can appear while generating the first 10 000 terms.
   The 64‑bit bound 5 000 is far above the real limit (≈120).              */
#define MAX_IJ 5000

/* visited bitmap: visited[i][j] == 1 means the pair (i,j) was already
   inserted into the heap.  It is statically allocated to avoid per‑call
   allocation overhead. */
static unsigned char visited[MAX_IJ + 1][MAX_IJ + 1];

/* Node used in the priority queue.  d is the logarithmic value. */
struct Node {
    uint64_t i;
    uint64_t j;
    double  d;
};

/* Heap that stores Nodes in ascending order of d.  The capacity is
   more than enough for the first 10 000 terms (≈20 000 nodes). */
#define HEAP_CAP 20000
static struct Node heap[HEAP_CAP];
static size_t heap_size = 0;

/* Heap helper functions -------------------------------------------------*/
static void sift_up(size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[idx].d >= heap[parent].d)
            break;
        struct Node tmp = heap[idx];
        heap[idx] = heap[parent];
        heap[parent] = tmp;
        idx = parent;
    }
}

static void sift_down(size_t idx)
{
    while (1) {
        size_t left = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;
        if (left < heap_size && heap[left].d < heap[smallest].d)
            smallest = left;
        if (right < heap_size && heap[right].d < heap[smallest].d)
            smallest = right;
        if (smallest == idx)
            break;
        struct Node tmp = heap[idx];
        heap[idx] = heap[smallest];
        heap[smallest] = tmp;
        idx = smallest;
    }
}

static void heap_push(struct Node n)
{
    if (heap_size >= HEAP_CAP) {
        /* This should never happen for the intended limits. */
        abort();
    }
    heap[heap_size++] = n;
    sift_up(heap_size - 1);
}

static struct Node heap_pop_min(void)
{
    struct Node min = heap[0];
    heap[0] = heap[--heap_size];
    sift_down(0);
    return min;
}

/* Memoisation cache -----------------------------------------------------*/
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void cache_expand(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main function -----------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined by OEIS offset, safe fallback */

    /* 1‑based indexing: a(1) is the first term. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Initialise the heap and visited bitmap on the first call. */
    static bool initialized = false;
    if (!initialized) {
        heap_size = 0;
        visited[0][0] = 1;
        struct Node root = {0, 0, 0.0};
        heap_push(root);
        /* Ensure cache capacity for at least one element. */
        cache_expand(1);
        initialized = true;
    }

    /* Logarithm constants (computed only once). */
    static const double LOG4 = log(4.0);
    static const double LOG7 = log(7.0);

    /* Generate terms until we have computed up to n. */
    while (cache_len < (size_t)n) {
        struct Node cur = heap_pop_min();
        cache[cache_len++] = (seq_t)cur.j;   /* store exponent j */

        /* Insert neighbour (i+1, j) if not yet visited. */
        if (cur.i + 1 <= MAX_IJ && !visited[cur.i + 1][cur.j]) {
            visited[cur.i + 1][cur.j] = 1;
            struct Node next = {cur.i + 1, cur.j, cur.d + LOG4};
            heap_push(next);
        }

        /* Insert neighbour (i, j+1) if not yet visited. */
        if (cur.j + 1 <= MAX_IJ && !visited[cur.i][cur.j + 1]) {
            visited[cur.i][cur.j + 1] = 1;
            struct Node next = {cur.i, cur.j + 1, cur.d + LOG7};
            heap_push(next);
        }

        /* Grow cache if needed. */
        if (cache_len >= cache_cap)
            cache_expand(cache_cap << 1);
    }

    return cache[n - 1];
}
