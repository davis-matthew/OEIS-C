/*
 * A077196  Smallest possible sum of the digits of a multiple of n.
 * https://oeis.org/A077196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9832, u16 n <= 9832, u32 n <= 9832, u64 n <= 9832,
 *            u128 n <= 9832, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation finds the minimal sum of decimal digits of a
   positive multiple of n (OEIS A077196).  The algorithm is a
   Dijkstra search on a directed graph whose vertices are the
   remainders modulo n.  From a remainder r we may append a digit d
   (0 ≤ d ≤ 9) yielding a new remainder (10*r + d) mod n and adding
   d to the digit‑sum cost.  The initial states correspond to
   one‑digit numbers 1…9.  The first time remainder 0 is extracted
   from the priority queue the smallest possible digit sum for
   a multiple of n is found. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/* priority‑queue entry */
struct HeapEntry {
    unsigned long long dist;  /* current digit sum */
    idx_t                rem;  /* remainder modulo n */
};

/* push an entry into a binary min‑heap */
static void heap_push(struct HeapEntry *heap, size_t *size, size_t capacity,
                      unsigned long long dist, idx_t rem) {
    if (*size >= capacity) return;                /* protect overflow */
    size_t i = *size;
    heap[i].dist = dist;
    heap[i].rem  = rem;
    ++(*size);
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].dist <= heap[i].dist) break;
        struct HeapEntry tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

/* pop the minimum entry from the heap */
static struct HeapEntry heap_pop(struct HeapEntry *heap, size_t *size) {
    struct HeapEntry res = heap[0];
    --(*size);
    if (*size == 0) return res;
    heap[0] = heap[*size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < *size && heap[l].dist < heap[smallest].dist) smallest = l;
        if (r < *size && heap[r].dist < heap[smallest].dist) smallest = r;
        if (smallest == i) break;
        struct HeapEntry tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 */

    /* Allocate distance and visited arrays */
    size_t len = (size_t)n;
    unsigned long long *dist = (unsigned long long *)malloc(len * sizeof(unsigned long long));
    bool *visited = (bool *)malloc(len * sizeof(bool));
    if (!dist || !visited) {
        free(dist); free(visited);
        return (seq_t)0;
    }
    for (size_t i = 0; i < len; ++i) {
        dist[i] = ULLONG_MAX;
        visited[i] = false;
    }

    /* Allocate heap with generous capacity (10 per state is safe) */
    size_t heap_cap = len * 10 + 5;
    struct HeapEntry *heap = (struct HeapEntry *)malloc(heap_cap * sizeof(struct HeapEntry));
    if (!heap) {
        free(dist); free(visited);
        return (seq_t)0;
    }
    size_t heap_size = 0;

    /* Initialise with one‑digit numbers 1…9 */
    for (int d = 1; d <= 9; ++d) {
        idx_t r = (idx_t)d % n;
        if ((unsigned long long)d < dist[(size_t)r]) {
            dist[(size_t)r] = (unsigned long long)d;
            heap_push(heap, &heap_size, heap_cap,
                      dist[(size_t)r], r);
        }
    }

    /* Main Dijkstra loop */
    while (heap_size > 0) {
        struct HeapEntry cur = heap_pop(heap, &heap_size);
        size_t u = (size_t)cur.rem;
        if (visited[u]) continue;
        visited[u] = true;
        if (u == 0) {      /* found minimal sum for multiple of n */
            unsigned long long result = cur.dist;
            free(dist); free(visited); free(heap);
            return (seq_t)result;
        }
        unsigned long long curdist = cur.dist;
        for (int d = 0; d <= 9; ++d) {
            idx_t v = ((10ULL * (idx_t)u) + (idx_t)d) % n;
            size_t vi = (size_t)v;
            unsigned long long ndist = curdist + (unsigned long long)d;
            if (ndist < dist[vi]) {
                dist[vi] = ndist;
                heap_push(heap, &heap_size, heap_cap, ndist, v);
            }
        }
    }

    /* If the loop ends without reaching remainder 0, return 0 as error */
    free(dist); free(visited); free(heap);
    return (seq_t)0;
}
