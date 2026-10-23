/*
 * A025672  Exponent of 8 (value of j) in n-th number of form 3^i*8^j.
 * https://oeis.org/A025672
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

/*  OEIS A025672 – Exponent of 8 in the nth number of the form 3^i·8^j.
    The sequence lists j where the numbers 3^i·8^j are taken in increasing
    order.  Implementation uses a min‑heap of logarithmic values to avoid
    big‑integer arithmetic, and memoises results so that repeated calls
    are fast. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

/* maximum exponents needed for n up to 10 000 – a generous safety bound */
#define MAX_EXP 512

/* visited flag to avoid pushing the same (i , j) pair twice */
static bool visited[MAX_EXP][MAX_EXP] = {0};

/* a node for the heap: value is the logarithm of 3^i·8^j so that order is
   determined exactly, i and j are the exponents.  logv is stored as long
   double for extra precision. */
typedef struct {
    long double logv;
    unsigned int i;
    unsigned int j;
} HeapNode;

/* binary min‑heap of HeapNode */
typedef struct {
    HeapNode *data;
    size_t size;
    size_t capacity;
} Heap;

static Heap heap = {NULL, 0, 0};

/* heap helper: double capacity when needed */
static void heap_expand(void)
{
    size_t new_cap = heap.capacity ? heap.capacity * 2 : 256;
    size_t max_cap = MAX_EXP * MAX_EXP;
    if (new_cap > max_cap) new_cap = max_cap;
    HeapNode *new_data = (HeapNode *)realloc(heap.data, new_cap * sizeof(HeapNode));
    heap.data = new_data;
    heap.capacity = new_cap;
}

/* sift‑up after insertion */
static void heap_sift_up(size_t pos)
{
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap.data[parent].logv <= heap.data[pos].logv)
            break;
        HeapNode tmp = heap.data[parent];
        heap.data[parent] = heap.data[pos];
        heap.data[pos] = tmp;
        pos = parent;
    }
}

/* sift‑down after removal */
static void heap_sift_down(size_t pos)
{
    while (1) {
        size_t left = (pos << 1) + 1;
        size_t right = left + 1;
        size_t smallest = pos;
        if (left < heap.size && heap.data[left].logv < heap.data[smallest].logv)
            smallest = left;
        if (right < heap.size && heap.data[right].logv < heap.data[smallest].logv)
            smallest = right;
        if (smallest == pos) break;
        HeapNode tmp = heap.data[pos];
        heap.data[pos] = heap.data[smallest];
        heap.data[smallest] = tmp;
        pos = smallest;
    }
}

/* push a new node onto the heap */
static void heap_push(HeapNode node)
{
    if (heap.size == heap.capacity) heap_expand();
    heap.data[heap.size++] = node;
    heap_sift_up(heap.size - 1);
}

/* pop the smallest node */
static HeapNode heap_pop(void)
{
    HeapNode top = heap.data[0];
    heap.data[0] = heap.data[--heap.size];
    heap_sift_down(0);
    return top;
}

/* memoised cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

static void cache_append(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 256;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* main function required by the test harness */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* below offset – may return 0 */

    /* initialise on first call */
    if (cache_size == 0) {
        visited[0][0] = true;
        heap_push((HeapNode){.logv = 0.0L, .i = 0u, .j = 0u});
    }

    /* generate terms up to n */
    static const long double LOG_3 = logl(3.0L);
    static const long double LOG_8 = logl(8.0L);

    while (cache_size < n) {
        HeapNode cur = heap_pop();

        /* store exponent of 8 */
        cache_append((seq_t)cur.j);

        /* generate next states, guarding against overflow */
        if (cur.i + 1 < MAX_EXP && !visited[cur.i + 1][cur.j]) {
            visited[cur.i + 1][cur.j] = true;
            heap_push((HeapNode){.logv = cur.logv + LOG_3,
                                 .i = cur.i + 1u,
                                 .j = cur.j});
        }
        if (cur.j + 1 < MAX_EXP && !visited[cur.i][cur.j + 1]) {
            visited[cur.i][cur.j + 1] = true;
            heap_push((HeapNode){.logv = cur.logv + LOG_8,
                                 .i = cur.i,
                                 .j = cur.j + 1u});
        }
    }

    return cache[n - 1];
}
