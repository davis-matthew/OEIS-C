/*
 * A025632  Numbers of form 7^i*10^j, with i, j >= 0.
 * https://oeis.org/A025632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 66, u64 n <= 241, u128 n <= 921,
 *            big any n
 * Helpers:   mul_checked
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers of form 7^i * 10^j (i,j ≥ 0) sorted increasingly.
   Algorithm: generate all such numbers in order using a
   min‑heap; skip a child if its value would overflow the
   concrete seq_t type. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mul_checked.h"

/* ------------------------------------------------------------------ */
/* Helper to multiply two unsigned values with overflow detection.    */

/* ------------------------------------------------------------------ */
/* Node in the heap: current value and its exponents i (7^i) and j (10^j) */
typedef struct {
    seq_t val;
    idx_t i;
    idx_t j;
} Node;

/* ------------------------------------------------------------------ */
/* Visited pair (i,j) to avoid duplicates */
typedef struct {
    idx_t i;
    idx_t j;
} Pair;

/* ------------------------------------------------------------------ */
/* Global state for memoisation and the generation machinery */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

static Node *heap        = NULL;
static size_t heap_len   = 0;
static size_t heap_cap   = 0;

static Pair *visited     = NULL;
static size_t visited_len = 0;
static size_t visited_cap = 0;

/* ------------------------------------------------------------------ */
/* Utility: linear search in visited set */
static bool visited_contains(idx_t i, idx_t j)
{
    for (size_t k = 0; k < visited_len; ++k) {
        if (visited[k].i == i && visited[k].j == j)
            return true;
    }
    return false;
}

/* Utility: add to visited set */
static void visited_add(idx_t i, idx_t j)
{
    if (visited_len == visited_cap) {
        visited_cap = visited_cap ? visited_cap * 2 : 8;
        visited = (Pair *)realloc(visited, visited_cap * sizeof(Pair));
    }
    visited[visited_len].i = i;
    visited[visited_len].j = j;
    ++visited_len;
}

/* ------------------------------------------------------------------ */
/* Heap operations (min‑heap based on val) */
static void heap_push(idx_t i, idx_t j, seq_t val)
{
    if (heap_len == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 8;
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
    }
    heap[heap_len].i = i;
    heap[heap_len].j = j;
    heap[heap_len].val = val;
    size_t pos = heap_len++;
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[parent].val <= heap[pos].val)
            break;
        Node tmp = heap[parent];
        heap[parent] = heap[pos];
        heap[pos]   = tmp;
        pos = parent;
    }
}

static Node heap_pop(void)
{
    Node res = heap[0];
    --heap_len;
    if (heap_len > 0) {
        heap[0] = heap[heap_len];
        size_t pos = 0;
        for (;;) {
            size_t left = (pos << 1) + 1;
            if (left >= heap_len) break;
            size_t right = left + 1;
            size_t smallest = left;
            if (right < heap_len && heap[right].val < heap[left].val)
                smallest = right;
            if (heap[pos].val <= heap[smallest].val)
                break;
            Node tmp = heap[pos];
            heap[pos] = heap[smallest];
            heap[smallest] = tmp;
            pos = smallest;
        }
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; callers should not use n==0 */
        return (seq_t)0;

    /* Generate terms until we have at least n cached */
    while (cache_len < n) {
        /* Initialise heap with the first term if empty */
        if (heap_len == 0) {
            heap_push(0, 0, (seq_t)1);
            visited_add(0, 0);
        }

        Node cur = heap_pop();

        /* Store value in cache */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 8;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = cur.val;

        /* Generate children: (i+1, j) *7  and  (i, j+1) *10 */
        idx_t ni, nj;
        seq_t child_val;
        bool ofl;

        /* Child: multiply by 7 */
        ni = cur.i + 1;
        nj = cur.j;
        if (!visited_contains(ni, nj)) {
            visited_add(ni, nj);
            mul_checked(cur.val, 7U, &child_val, &ofl);
            if (!ofl)
                heap_push(ni, nj, child_val);
        }

        /* Child: multiply by 10 */
        ni = cur.i;
        nj = cur.j + 1;
        if (!visited_contains(ni, nj)) {
            visited_add(ni, nj);
            mul_checked(cur.val, 10U, &child_val, &ofl);
            if (!ofl)
                heap_push(ni, nj, child_val);
        }
    }

    return cache[n - 1];
}
