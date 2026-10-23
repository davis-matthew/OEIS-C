/*
 * A025652  Exponent of 5 (value of i) in n-th number of form 5^i*7^j.
 * https://oeis.org/A025652
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

/* Sequence A025652: exponent of 5 in the n‑th number of the form 5^i*7^j */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* constants for the logarithms of 5 and 7 (used for ordering) */
static const double LOG5 = 1.609437912434100374600759333226; /* loge(5) */
static const double LOG7 = 1.945910149055313305108883115877; /* loge(7) */

/* limits – sufficient for n up to about 10 000 */
#define MAX_EXP   2000      /* maximal i or j expected */
#define MAX_HEAP  40000     /* size of the priority queue */

/* node stored in the heap: log value + exponents */
typedef struct {
    double logv;
    uint16_t i;
    uint16_t j;
} Node;

/* static global state used for memoisation */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_capacity = 0;

/* heap implementation */
static Node heap[MAX_HEAP];
static size_t heap_size = 0;

static void heap_swap(size_t a, size_t b)
{
    Node tmp = heap[a];
    heap[a] = heap[b];
    heap[b] = tmp;
}

static void heap_push(double logv, uint16_t i, uint16_t j)
{
    size_t idx = heap_size++;
    heap[idx].logv = logv;
    heap[idx].i    = i;
    heap[idx].j    = j;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].logv <= heap[idx].logv)
            break;
        heap_swap(parent, idx);
        idx = parent;
    }
}

static void heap_pop(Node *out)
{
    *out = heap[0];
    heap_size--;
    heap[0] = heap[heap_size];
    size_t idx = 0;
    while (true) {
        size_t left  = (idx << 1) + 1;
        size_t right = left + 1;
        if (left >= heap_size)
            break;
        size_t smallest = left;
        if (right < heap_size && heap[right].logv < heap[left].logv)
            smallest = right;
        if (heap[idx].logv <= heap[smallest].logv)
            break;
        heap_swap(idx, smallest);
        idx = smallest;
    }
}

/* visited table to avoid pushing the same (i,j) twice */
static bool visited[MAX_EXP][MAX_EXP] = {false};

/* generate terms up to n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n)
        return;

    if (cache == NULL) {
        cache_capacity = n + 100; /* a small overhead */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    } else if (cache_capacity < n + 100) {
        cache_capacity = n + 100;
        cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
        if (cache == NULL) abort(); /* allocation failed */
    }

    /* initialise only once */
    static bool first = true;
    if (first) {
        visited[0][0] = true;
        heap_push(0.0, 0, 0);         /* value 1 = 5^0 * 7^0 */
        first = false;
    }

    while (cache_size < n) {
        Node cur;
        heap_pop(&cur);
        cache[cache_size++] = (seq_t)cur.i;

        /* push (i+1, j) */
        if (cur.i + 1 < MAX_EXP) {
            uint16_t ni = cur.i + 1;
            uint16_t nj = cur.j;
            if (!visited[ni][nj]) {
                visited[ni][nj] = true;
                heap_push(cur.logv + LOG5, ni, nj);
            }
        }

        /* push (i, j+1) */
        if (cur.j + 1 < MAX_EXP) {
            uint16_t ni = cur.i;
            uint16_t nj = cur.j + 1;
            if (!visited[ni][nj]) {
                visited[ni][nj] = true;
                heap_push(cur.logv + LOG7, ni, nj);
            }
        }
    }
}

/* public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1 – the behaviour for 0 is irrelevant */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n - 1];
}
