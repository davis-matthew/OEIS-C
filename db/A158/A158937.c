/*
 * A158937  Numbers of the form x^2+3y^2 where x and y are positive integers
 *          (with repetitions).
 * https://oeis.org/A158937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   val_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A158937: Numbers of the form x^2 + 3y^2, x,y ≥ 1, sorted with repetitions. */
/* The algorithm maintains a min‑heap of (value, x, y) entries and a static
   cache of all computed terms. Each call to a(n) extends the cache if
   necessary, taking O(n log n) time for generating the first n terms. */

#include <stdint.h>  /* uint64_t, etc. */
#include <stdlib.h>  /* malloc, realloc, free */
#include <stdbool.h>
#include "../utils/val_of.h"

typedef struct {
    seq_t val;
    uint64_t x;
    uint64_t y;
} Node;

/* Min‑heap helpers ------------------------------------------------------- */

static void bubble_up(Node *h, size_t size, size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (h[idx].val >= h[parent].val)
            break;
        Node tmp = h[idx];
        h[idx] = h[parent];
        h[parent] = tmp;
        idx = parent;
    }
}

static void heapify_down(Node *h, size_t size, size_t idx)
{
    while (1) {
        size_t left  = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;

        if (left < size && h[left].val < h[smallest].val)
            smallest = left;
        if (right < size && h[right].val < h[smallest].val)
            smallest = right;
        if (smallest == idx)
            break;
        Node tmp = h[idx];
        h[idx] = h[smallest];
        h[smallest] = tmp;
        idx = smallest;
    }
}

static void heap_push(Node **hp, size_t *psize, size_t *pcap,
                      seq_t v, uint64_t x, uint64_t y)
{
    if (*psize == *pcap) {
        *pcap = (*pcap == 0) ? 16 : (*pcap << 1);
        *hp = (Node*)realloc(*hp, *pcap * sizeof(Node));
    }
    size_t idx = *psize;
    (*hp)[idx].val = v;
    (*hp)[idx].x   = x;
    (*hp)[idx].y   = y;
    bubble_up(*hp, *psize + 1, idx);
    (*psize)++;
}

static Node heap_pop(Node *h, size_t *psize)
{
    Node res = h[0];
    h[0] = h[*psize - 1];
    (*psize)--;
    heapify_down(h, *psize, 0);
    return res;
}

/* Static state for incremental generation -------------------------------- */

static Node *heap = NULL;
static size_t heap_size = 0;
static size_t heap_cap  = 0;
static uint64_t next_x  = 2;  /* next x to insert with y = 1 */

static seq_t *cache  = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Main function ----------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for n=0, but return 0 */
        return 0;

    /* grow cache array to hold at least n elements */
    if (n > cache_cap) {
        cache_cap = (cache_cap == 0) ? 16 : cache_cap;
        while (cache_cap < n) cache_cap <<= 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* extend cache if needed */
    while (cache_len < n) {
        /* initialise heap on first call */
        if (heap_size == 0 && heap_cap == 0) {
            heap_cap = 16;
            heap = (Node*)malloc(heap_cap * sizeof(Node));
            heap_push(&heap, &heap_size, &heap_cap,
                      val_of(1, 1), 1, 1);
            next_x = 2;
        }

        /* extract smallest element */
        Node cur = heap_pop(heap, &heap_size);
        cache[cache_len++] = cur.val;

        /* push next y for the same x */
        heap_push(&heap, &heap_size, &heap_cap,
                  val_of(cur.x, cur.y + 1), cur.x, cur.y + 1);

        /* as long as the smallest unexplored x has value
           not exceeding the current heap minimum, insert it */
        while (heap_size > 0) {
            seq_t cand = val_of(next_x, 1);
            if (cand > heap[0].val)
                break;
            heap_push(&heap, &heap_size, &heap_cap,
                      cand, next_x, 1);
            next_x++;
        }
    }

    return cache[n - 1];
}
